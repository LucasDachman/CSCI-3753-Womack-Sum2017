# Problem Set \#1
### Due June 19, 23:55 (11:55PM)

Upload a pdf with all answers. Ensure that you have clean/clear drawings for the last two questions.

1. **[10 Points]** What is the role of instructions such as `trap()` in operating system design?

   *Note:* This question is a little broad in its wording. If they describe the `trap()` command then thats what I was looking for.

   ***Answer:***
   A trap is a software-generated interrupt. A trap can be used to call operating system routines or to catch arithmetic errors. traps allow the operating system to switch user space system calls to kernel space.


2. **[10 Points]** What role does the trap table play in executing a system call?

   ***Answer:***
   Once a system call executes, control is passed to the OS via an appropriately parameterized trap instruction, which causes the OS kernel to run and consult the jump table to determine which handler code should be executed to process the system call, i.e. to which handler should the OS jump.

3. **[15 Points]** Provide a step-by-step description for adding a new device in Linux operating system without requiring recompiling the kernel.

   ***Answer:***
   1. Write the device driver, compile it to create an LKM (.ko file). This LKM contains an initialization routine `init_module()` function that registers various device functions contained in the LKM with the kernel using appropriate kernel functions such as `register_chrdev`, `register_blkdev`, etc. This registration fills the entry table in the kernel (`dev_func_i[j]`) with appropriate function pointers

   2. Call insmod command that gets a unique device number for the new device and invokes the init_module systems call to load the LKM. This system call invokes the LKM’s initialization routine.

4. **[15 Points]** Explain two ways that I/O can be overlapped with CPU execution and how they are each an improvement over not overlapping I/O with the CPU.

   ***Answer:***
   Interrupt driven I/O: CPU initiates the I/O and sets up an interrupt handler. After that it performs other useful work in parallel with I/O data transfer being performed by I/O device. When I/O data transfer is complete, the CPU is interrupted to complete the remaining work for completing the I/O. 

   DMA based I/O: Similar to interrupt-driven I/O with the addition that DMA controller manages the data transfer between memory and device registers. 

5. **[25 Points]** Draw and label a figure to show the sequence of steps (step 1, step 2, etc.) in a `write()` operation to disk, from the application first calling a `write()` through the OS processing the `write()` to the final return from the `write()` call upon completion of the disk operation. Assume DMA with interrupts. Further assume that the disk is not ready at first for the `write()` operation. Your answer should include components such as the device controller, interrupt handler, device handler, device driver, DMA controller and any other OS components you deem appropriate to add.

   ***Answer:***
![Screenshot 2017-06-21 16.07.39.png](resources/CA1808C535D7D9B6039590C7C92DEA2F.png =860x596)

6. **[25 Points]** Suppose we have a round-robin scheduler in the OS kernel of a preemptively time sliced system, and three programs in its ready queue: P1, P2, and P3. P1 takes 10 seconds to complete, P2 takes 8.5 seconds, and P3 takes 3 seconds. Suppose the time slice is 2 seconds, and the overhead for OS context switching is 0.2 seconds. If P1 starts executing first at time 0, when do each of the programs finish executing? Draw a timeline of execution. What is the percentage overhead due to context switching? Assume that a process that finishes early transfers control back to the OS scheduler.

<br>
<br>
<br>

   ***Answer:***
   
   |||||||
   |:---:|:---:|:---:|:---:|:---:|:---:|
   | P1: | 0-2 | 6.6-8.6 | 12.2-14.2 | 16.6-18.6 | 21-23 |
   | P2: | 2.2-4.2 | 8.8-10.8 | 14.4-16.4 | 18.8-20.8 | 23.2-23.7 | 
   | P3: | 4.4-6.4 | 11-12 | | | |
   ---
   
   P1 = 23 seconds
   P2 = 23.7 seconds
   P3 = 12 seconds
   % overhead = 0.2/(0.2+2) = 9.09