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

2. **[10 Points]** Suppose the `swap()` function above is called by the interrupt service routine (ISR) below.  Assume that if `swap()` is interrupted during normal execution, the ISR below is called and calls `swap()` again, reentering `swap()` and executing in the context of the interrupted thread.  Is `swap()` reentrant?  Explain your reasoning.

   ```c
   void interrupt_service_routine()
   {
       int a = 1, b = 2;
       swap(&a, &b);
   }
   ```
   
3. **[20 Points]** Explain why the solution to the 3rd Readers/Writers problem is starvation-free.

4. **[30 Points]** You have just been hired by Greenpeace to help the environment. Because unscrupulous commercial interests have dangerously lowered the whale population, whales are having synchronization problems in finding a mate. The trick is that in order to have children, three whales are needed, one male, one female, and one to play matchmaker --- literally, to push the other two whales together (I am not making this up!). Your job is to write three functions: `Male()`, `Female()`, and `Matchmaker()`. A male whale calls `Male()`, which waits until there is a waiting female and a matchmaker. A female whale calls `Female()`, which must wait until there is a waiting male and a matchmaker. Similarly, a matchmaker calls `Matchmaker()`, which must wait until there is a waiting male and a female. Once all three types of whales are present, all three return with one of them printing a message “A calf is born”. Use semaphores to implement the required synchronization

5. **[30 Points]** Unisex bathroom problem: CU wants to apply the U.S. Supreme Court’s “Separate but equal is inherently unequal” doctrine to gender, ending its long-standing practice of gender-segregated bathrooms on campus. However, as a concession to tradition, it decrees that when a woman is in the bathroom, other women may enter, but no men, and vice versa. Also, due to fire code, at most N (N > 1) individuals may use the bathroom at any time.

   You task is to write two functions: `man_use_bathroom()` and `woman_use_bathroom()`. Provide a monitor-based solution that manages access to the bathroom. Your solution should be fair, starvation free and deadlock free.