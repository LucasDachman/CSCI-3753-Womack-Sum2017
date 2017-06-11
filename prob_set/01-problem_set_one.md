# Problem Set \#1
### Due June 19, 23:55 (11:55PM)

Upload a pdf with all answers. Ensure that you have clean/clear drawings for the last two questions.

1. **[10 Points]** What is the role of instructions such as `trap()` in operating system design?

2. **[10 Points]** What role does the trap table play in executing a system call?

3. **[15 Points]** Provide a step-by-step description for adding a new device in Linux operating system without requiring recompiling the kernel.

4. **[15 Points]** Explain two ways that I/O can be overlapped with CPU execution and how they are each an improvement over not overlapping I/O with the CPU.

5. **[10 Points]** What role does the jump table play in executing a system call?

6. **[20 Points]** Draw and label a figure to show the sequence of steps (step 1, step 2, etc.) in a `write()` operation to disk, from the application first calling a `write()` through the OS processing the `write()` to the final return from the `write()` call upon completion of the disk operation. Assume DMA with interrupts. Further assume that the disk is not ready at first for the `write()` operation. Your answer should include components such as the device controller, interrupt handler, device handler, device driver, DMA controller and any other OS components you deem appropriate to add.

7. **[20 Points]** Suppose we have a round-robin scheduler in the OS kernel of a preemptively time sliced system, and three programs in its ready queue: P1, P2, and P3. P1 takes 10 seconds to complete, P2 takes 8.5 seconds, and P3 takes 3 seconds. Suppose the time slice is 2 seconds, and the overhead for OS context switching is 0.2 seconds. If P1 starts executing first at time 0, when do each of the programs finish executing? Draw a timeline of execution. What is the percentage overhead due to context switching? Assume that a process that finishes early transfers control back to the OS scheduler.
