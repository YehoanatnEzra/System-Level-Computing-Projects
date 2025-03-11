# **User-Level Thread Management Library**
This project is a user-level thread management library for a Linux-based operating system. It provides efficient thread management, including creation, scheduling, and termination, without relying on OS-level threading mechanisms.

## **Project Overview**  
This library manages user-level threads independently from the OS, supporting:
- **Thread creation, scheduling, blocking, and termination.**
- **Round-Robin scheduling** with a fixed quantum per thread.
- **Preemptive context switching** using signals and timers (`setitimer`, `sigaction`).
- **Sleep functionality**, allowing threads to suspend themselves for a specified number of quantums.
- **Optimized performance** using efficient data structures: a ready queue, a sleep map, and a min-heap for managing thread IDs.

## **Features**
- **Thread Lifecycle Management** – Create, block, resume, and terminate threads.  
- **Round-Robin Scheduling** – Threads execute cyclically with a fixed quantum.  
- **Preemptive Scheduling** – Automatic thread switching via timers.  
- **Sleep Functionality** – Threads can voluntarily sleep for a defined number of quantums.  
- **User-Level Implementation** – No dependency on the `pthread` library.  
---

## **API Reference**
**- int uthread_init(int quantum_usecs)** - Initializes the thread library with a quantum size (in microseconds) 
**- int uthread_spawn(thread_entry_point entry_point)** - Creates a new thread and adds it to the ready queue.  
**int uthread_block(int tid)** - Blocks a specified thread, preventing it from executing until resumed. 
**int uthread_resume(int tid)** - Resumes a previously blocked thread, allowing it to run again.  
**int uthread_sleep(int num_quantums)** - Makes the **currently running thread sleep** for `num_quantums.
**int uthread_terminate(int tid)** - Terminates the specified thread. If `tid == 0`, the entire process is terminated.  

## **Test Cases**  
The project includes externally written test cases that validate the correctness of the library. These tests:
- **Demonstrate API usage** in various scenarios.
- **Verify core functionalities**, including scheduling, blocking, sleeping, and termination.
- **Are located in the repository** and can be executed as described above.
