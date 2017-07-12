# Problem Set \#3
### Due July 3, 23:55 (11:55PM)

1. **[10 Points]** Is the `swap()` function below thread-safe or not?  Explain your reasoning.

   ```c
   int temp;

   void swap(int *y, int *z)
   {
       int local;

       local = temp;
       temp = *y;
       *y = *z;
       *z = temp;
       temp = local;

   }
   ```

   **Answer:** Swap is not thread-safe, because it is using a global variable temp. For example, suppose thread T1 called swap(&m,&n), where `*m=1` and `*n=2`, and thread T2 called `swap(&q,&r)` where `*q=7` and `*r=4`. Let T1 execute inside `swap()`, up to just before the line `*z=temp`. At this point, `temp=*y=1`, and T1 desires to set `*z=1` to complete the swap. But if T1 is interrupted at this point, then T2 executes, then T2 sets `temp=*y=7`. Now suppose T1 context switches back in, and executes `*z=temp=7`. So the output of the `swap()` does not swap the values 1 and 2, but instead produces a value of 7 for one of the variables. Hence, the code is not thread safe.

2. **[10 Points]** Suppose the `swap()` function above is called by the interrupt service routine (ISR) below.  Assume that if `swap()` is interrupted during normal execution, the ISR below is called and calls `swap()` again, reentering `swap()` and executing in the context of the interrupted thread.  Is `swap()` reentrant?  Explain your reasoning.

   ```c
   void interrupt_service_routine()
   {
       int a = 1, b = 2;
       swap(&a, &b);
   }
   ```

   **Answer:** This interrupt service routine is reentrant. We have to imagine two control flows: normal control flow of the program's execution, which calls `swap()`; and exceptional control flow caused by say an interrupt, which triggers the interrupt service routine to run, which also calls `swap()`.  

   Suppose the first call to `swap()` by normal control flow of the executing program was to swap two numbers, say 7 and 10. Assume the entry value of the global variable temp was 5. The expectation at the end of the routine is that the global variable is returned to its initial value 5, and that the two numbers 7 and 10 are swapped correctly.  

   Now suppose in the middle of `swap()` an interrupt occurs. This would eventually trigger the `interrupt_service_routine()` to execute, which would call `swap()` again, reentering it with new values of 1 and 2. We assume in terms of reentrancy analysis that only this one thread can modify the global variable temp, and there are no other threads. The value of the global variable temp may no longer be 5 due to the swap proceeding, e.g. if the `swap()` is interrupted after the line `temp=*y`, then temp's value is no longer 5, and in this example would be 7. The second call to `swap()` must swap 1 and 2, and restore the global variable of temp upon exit to 7. The 2nd call to `swap()` has its own local variable, which is set to 7 to save the global variable. The middle three lines swap the values 1 and 2 correctly using temp, then temp is restored correctly on exit by `temp=local`.  

   Does the exceptional control flow's reentry of `swap()` adversely affect the normal control flow's execution in `swap()`? No, because the normal control flow has its own local variable and own parameters passed into it on the stack, namely the values 7 and 10. Its local variable takes a snapshot of the current value of global temp as it entered, which is 5. If `swap()` is interrupted at any point thereafter, the exceptional control flow will completely finish before the normal control flow is allowed to resume execution, and the exceptional control flow will restore the value of temp to what it was at the point of interruption, say 7, as we saw above.  Therefore, the normal control flow's `swap()` will proceed using the correct value of temp to correctly complete the swap of the numbers 7 and 10, and restore the global temp to 5 upon exit. The code is reentrant.

3. **[20 Points]** Explain why the solution to the 3rd Readers/Writers problem is starvation-free.
   
   **Answer:** The starvation-free solution to the 2nd R/W problem, also called the 3rd R/W problem, simply adds a readBlock semaphore initialized to 1 to the 1st R/W problem. The solution is given on slide 27 of the Chapter 5.3 slides, repeated below. The 1st R/W problem is already starvation-free for readers, but not for writers. The addition of readBlock creates a solution that is also starvation-free for writers. This is because if a writer arrives while there are multiple current readers, then the writer will grab readBlock and sleep on wrt, which is held by the current readers. Future readers will block on readBlock. When the last current reader is done, it will signal the wrt lock, which wakes the writer so it can proceed. When the writer is done, it signals readBlock, which wakes any blocked readers so they can start reading. Thus, writers are no longer starved.

   ```c
   Semaphore wrt=1, mutex=1, readBlock=1;
   int readcount=0;
   ```

   **Writer:**
   ```c
   while(1) {
      wait(readBlock);
         wait(wrt);
  
         // writing
         
         signal(wrt);
      signal(readBlock);
   
   }
   ```

   **Reader:**
   ```c
   while(1) {
      wait(readBlock);
         wait(mutex);
            readcount++;
            if (readcount==1) wait(wrt);
         signal(mutex);
      signal(readBlock);
      
      // reading
      
      wait(mutex);
         readcount--;
         if (readcount==0) signal(wrt);
      signal(mutex);
   }
   ```

4. **[30 Points]** You have just been hired by Greenpeace to help the environment. Because unscrupulous commercial interests have dangerously lowered the whale population, whales are having synchronization problems in finding a mate. The trick is that in order to have children, three whales are needed, one male, one female, and one to play matchmaker --- literally, to push the other two whales together (I am not making this up!). Your job is to write three functions: `Male()`, `Female()`, and `Matchmaker()`. A male whale calls `Male()`, which waits until there is a waiting female and a matchmaker. A female whale calls `Female()`, which must wait until there is a waiting male and a matchmaker. Similarly, a matchmaker calls `Matchmaker()`, which must wait until there is a waiting male and a female. Once all three types of whales are present, all three return with one of them printing a message “A calf is born”. Use semaphores to implement the required synchronization

   **Answer:**
   
   ```c
   semaphore male = 0, female = 0, matchmaker = 0; 
   semaphore male_start = 0, male_end = 0; 
   semaphore female_start = 0, female_end = 0; 
   semaphore mutex = 1;
   
   Male()
   {
      signal(male); 
      wait(male_start);
      
      // Make a baby
      
      wait(male_end); 
      signal(matchmaker);
   }
   
   Female()
   {
      signal(female);
      wait(female_start);
      
      // Make a baby
      
      wait(female_end);
      signal(matchmaker);
   }
   
   Matchmaker()
   {
      wait(male);
      wait(female);
      wait(mutex);
      signal(male_start);
      signal(female_start);
      
      printf (“A calf is born\n”);
      
      signal(male_end);
      signal(female_end);
      wait(matchmaker);
      wait(matchmaker);
      signal(mutex);
   }
   ```

5. **[30 Points]** Unisex bathroom problem: CU wants to apply the U.S. Supreme Court’s “Separate but equal is inherently unequal” doctrine to gender, ending its long-standing practice of gender-segregated bathrooms on campus. However, as a concession to tradition, it decrees that when a woman is in the bathroom, other women may enter, but no men, and vice versa. Also, due to fire code, at most N (N > 1) individuals may use the bathroom at any time.

   You task is to write two functions: `man_use_bathroom()` and `woman_use_bathroom()`. Provide a monitor-based solution that manages access to the bathroom. Your solution should be fair, starvation free and deadlock free.
   
   **Answer:**

   ```c
   void man_use_bathroom() {
     unisex_bathroom.enter_bathroom_man();
     
     // Use restroom
     
     unisex_bathroom.exit_bathroom_man(); 
   }
   
   void woman_use_bathroom() {
     unisex_bathroom.enter_bathroom_woman();
     
     //Use restroom
     
     unisex_bathroom.exit_bathroom_woman(); 
   }
   
   monitor unisex_bathroom
   { 
     int mc, mcw, fc, fcw;
     condition m_cond, f_cond;

     void enter_bathroom_man
     {
       // no women in the bathroom, no women waiting, and # of men in bathroom is < N, enter bathroom 
       if (fc == 0 && fcw == 0 && mc < N)
       {
         mc++;
       }
       else
       {
         // have to wait 
         mcw++;
         m_cond.wait();
         mcw--;
         mc++;
         
         // check if more men waiting and mc < N; if so wake one man
         if (mcw > 0 && mc < N)
         {
           m_cond.signal();
         }
       }
     }
   
     // void enter_bathroom_woman is similar to enter_bathroom_man()
     void exit_bathroom_female
     {
       fc--; // one less woman in the bathroom now //no men waiting and at least one woman waiting
       if (mcw == 0 && fcw > 0)
       {
         f_cond.signal();
       }
       // some men waiting and no more women in the bathroom
       else if (mcw > 0 && fc == 0)
       {
         m_cond.signal();
       }
     }
     
     void exit_bathroom_male
     {
       mc--; // one less man in the bathroom now 
       //no women waiting and at least one man waiting
       if (fcw == 0 && mcw > 0)
       {
         m_cond.signal();
       }
       // some women waiting and no more men in the bathroom
       else if (fcw > 0 && mc == 0)
       {
         f_cond.signal();
       }
     }
   ```
   
   ###Explination 
   
   So we will begin with the situation where no one is in the bathroom and the max people who can be in the bathroom will be N=3.
   
   | Bathroom | Waiting Queue |
   |:---:|:---:|
   |  |  |
   
   | Variables | Values |
   |:---:|:---:|
   | mcw | 0 |
   | fcw | 0 |
   | mc | 0 |
   | fc | 0 |
   
   Now a man will enter the bathroom
   
      ```c
   void man_use_bathroom() {
     unisex_bathroom.enter_bathroom_man();
     
     // Use restroom
     
     unisex_bathroom.exit_bathroom_man(); 
   }
   ```
   We do this by calling `man_use_bathroom()`. This will call the `enter_bathroom_man()` method on the unisex bathroom. Since there is no one in the bathroom and no females waiting; we enter this portion of code in `enter_bathroom_man()`:
   ```c
   // no women in the bathroom, no women waiting, and # of men in bathroom is < N, enter bathroom
   if (fc == 0 && fcw == 0 && mc < N)
   {
     mc++;
   }
   ```
   While this man is in the bathroom another man comes up to enter the restroom. Again we will enter this portion of code
   ```c
   // no women in the bathroom, no women waiting, and # of men in bathroom is < N, enter bathroom
   if (fc == 0 && fcw == 0 && mc < N)
   {
     mc++;
   }
   ```
   This happens a third time at which point the bathroom will look like this: (each M is 1 male, and each F is 1 female)
   
   | Bathroom | Waiting Queue |
   |:---:|:---:|
   | MMM |  |
   
   | Variables | Values |
   |:---:|:---:|
   | mcw | 0 |
   | fcw | 0 |
   | mc | 3 |
   | fc | 0 |
   
   Now another man comes up as the others are executing the `// Use restroom` portion of `man_use_bathroom()`. Because there are 3 men in the bathroom the fourth man to call `unisex_bathroom.enter_bathroom_man()` will enter this portion of the code:
   ```c
   // either there are women in the bathroom
   // or there are women waiting
   // or there are too many men in the bathroom (this is the case at this point)
   else
   {
     // have to wait 
     mcw++;
     m_cond.wait();
     mcw--;
     mc++;
     
     // check if more men waiting and mc < N; if so wake one man
     if (mcw > 0 && mc < N)
     {
       m_cond.signal();
     }
   }
   ```
   The man will hit the `m_cond.wait()` condition variable and wait for a man to exit the bathroom and exit. So at this point the bathroom looks like this:
   
   | Bathroom | Waiting Queue |
   |:---:|:---:|
   | MMM | M |
    
   | Variables | Values |
   |:---:|:---:|
   | mcw | 1 |
   | fcw | 0 |
   | mc | 3 |
   | fc | 0 |
   
   Then one man will exit the `// Use restroom` portion of `man_use_bathroom()`. This will call `unisex_bathroom.exit_bathroom_male()` and enter this portion of the code:
   ```c
   mc--; // one less man in the bathroom now
   if (fcw == 0 && mcw > 0)
   {
     m_cond.signal();
   }
   ```
   
   This will send the `m_cond.signal()` to `m_cond.wait()` in the `unisex_bathroom.enter_bathroom_male()` method and "wakeup" the waiting man to enter the bathroom, which will look like this:
   
   | Bathroom | Waiting Queue |
   |:---:|:---:|
   | MMM |  |
   
   | Variables | Values |
   |:---:|:---:|
   | mcw | 0 |
   | fcw | 0 |
   | mc | 3 |
   | fc | 0 |
   
   This execution will be similar for the all female case as well. But earlier we saw that there may be an issue with starvation. This solution actually accounts for that. Lets start with the same situation above:
   
   | Bathroom | Waiting Queue |
   |:---:|:---:|
   | MMM |  |
   
   | Variables | Values |
   |:---:|:---:|
   | mcw | 0 |
   | fcw | 0 |
   | mc | 3 |
   | fc | 0 |
   
   Now a woman shows up and executes `woman_use_bathroom()` which in turn executes `unisex_bathroom.enter_bathroom_female()` which will enter this portion of code:
   ```c
   // either there are men in the bathroom  (this is the case at this point)
   // or there are men waiting
   // or there are too many women in the bathroom
   else
   {
     // have to wait 
     fcw++;
     f_cond.wait();
     fcw--;
     fc++;
     
     // check if more men waiting and mc < N; if so wake one man
     if (fcw > 0 && fc < N)
     {
       f_cond.signal();
     }
   }
   ```
   Once the `f_cond.wait()` at line 8 is executed the method will block and the woman will be waiting in the queue:
   
   | Bathroom | Waiting Queue |
   |:---:|:---:|
   | MMM | F |
   
   | Variables | Values |
   |:---:|:---:|
   | mcw | 0 |
   | fcw | 1 |
   | mc | 3 |
   | fc | 0 |
   
   Now we said that if another male shows up he will get prefernce over the already waiting female causing starvation to occur so long as men continue to show up. This turns out to not be the case. Lets see how. A man shows up calling `man_use_bathroom()` which calls the `enter_bathroom_male()`. We will again be in this section:
   ```c
   // either there are women in the bathroom
   // or there are women waiting (this is the case at this point)
   // or there are too many men in the bathroom (and this is the case at this point)
   else
   {
     // have to wait 
     mcw++;
     m_cond.wait();
     mcw--;
     mc++;
     
     // check if more men waiting and mc < N; if so wake one man
     if (mcw > 0 && mc < N)
     {
       m_cond.signal();
     }
   }
   ```
   The man will have to wait when line 8 is executed. 
   
   | Bathroom | Waiting Queue |
   |:---:|:---:|
   | MMM | FM |
   
   | Variables | Values |
   |:---:|:---:|
   | mcw | 1 |
   | fcw | 1 |
   | mc | 3 |
   | fc | 0 |
   
   Now we will start having men exit the bathroom and see what happens. This will call `unisex_bathroom.exit_bathroom_male()` and hit this portion of the code:
   ```c
   void exit_bathroom_male
   {
     mc--; // one less man in the bathroom now 
   }
   ```
   
   Now why didn't the conditions that signal the waits execute when the man exited the bathroom. Here is the whole function
   ```c
   void exit_bathroom_male
   {
     mc--; // one less man in the bathroom now 
     //no women waiting and at least one man waiting
     if (fcw == 0 && mcw > 0)
     {
       m_cond.signal();
     }
     // some women waiting and no more men in the bathroom
     else if (fcw > 0 && mc == 0)
     {
       f_cond.signal();
     }
   }
   ```
   
   Note that the conditions are `if (fcw == 0 && mcw > 0)` and `else if (fcw > 0 && mc == 0)`. Looking at the variables above we can see that `fcw==1` and  `mc==2` (after the `mc--`) which doesn't allow either case to execute. So we are left with a bathroom that looks like this:
   
   | Bathroom | Waiting Queue |
   |:---:|:---:|
   | MM | FM |
   
   | Variables | Values |
   |:---:|:---:|
   | mcw | 1 |
   | fcw | 1 |
   | mc | 2 |
   | fc | 0 |
   
   Now the other male exits in the same way leaving us with:
   
   | Bathroom | Waiting Queue |
   |:---:|:---:|
   | M | FM |
   
   | Variables | Values |
   |:---:|:---:|
   | mcw | 1 |
   | fcw | 1 |
   | mc | 1 |
   | fc | 0 |
   
   Then the last male exits and we do enter a case:
   ```c
   void exit_bathroom_male
   {
     mc--; // one less man in the bathroom now 
     // some women waiting and no more men in the bathroom
     else if (fcw > 0 && mc == 0)
     {
       f_cond.signal();
     }
   }
   ```
   
   We bypass the first case (`if (fcw == 0 && mcw > 0)`) becuase there is a female waiting and execute the second case because we `mc--` at the begining of the function leaves no one in the bathroom and a female waiting (`else if (fcw > 0 && mc == 0)`):
   
   | Bathroom | Waiting Queue |
   |:---:|:---:|
   | F | M |
   
   | Variables | Values |
   |:---:|:---:|
   | mcw | 1 |
   | fcw | 0 |
   | mc | 0 |
   | fc | 1 |
   
   Note that if another female shows up the same thing will happen and we will begin alternating between male and female. Thus no starvation. But we also aren't using every space in the restroom. Actually we are with this portion of the code in void `enter_bathroom_male()`:
   ```c
   // check if more men waiting and mc < N; if so wake one man
   if (mcw > 0 && mc < N)
   {
     m_cond.signal();
   }
   ```
   
   Lets look at a quick example:
   
   | Bathroom | Waiting Queue |
   |:---:|:---:|
   | FFF | MFMFMF |
   
   | Variables | Values |
   |:---:|:---:|
   | mcw | 3 |
   | fcw | 3 |
   | mc | 0 |
   | fc | 3 |
   
   Now two female will `exit_bathroom_female()` without signaling anyone because there are men waiting and there is still one female in the bathroom:
   
   | Bathroom | Waiting Queue |
   |:---:|:---:|
   | F | MFMFMF |
   
   | Variables | Values |
   |:---:|:---:|
   | mcw | 3 |
   | fcw | 3 |
   | mc | 0 |
   | fc | 1 |
   
   Then when the last female exits she will signal to the first waiting male (check the conditions) to enter, which will then run this portion of code in `enter_bathroom_male()`:
   ```c
   // check if more men waiting and mc < N; if so wake one man
   if (mcw > 0 && mc < N)
   {
     m_cond.signal();
   }
   ```
   This will wake up the 2nd waiting male who will also execute this portion of code waking the 3rd waiting male. This will stop after `mc == N`. 
   
   | Bathroom | Waiting Queue |
   |:---:|:---:|
   | MMM | FFF |
   
   | Variables | Values |
   |:---:|:---:|
   | mcw | 0 |
   | fcw | 3 |
   | mc | 3 |
   | fc | 0 |   
   
   After this point we begin repeating previous steps where 2 male exit signaling no one and the last male to exit signals the first female. Even if male shows up before the last male exits, the men exiting the bathroom won't signal another male to enter the bathroom. 
