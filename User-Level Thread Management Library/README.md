# **User-Level Thread Management Library**
This project is a user-level thread management library for Linux-based operating systems. It provides efficient thread management, including thread creation, scheduling, blocking, and termination, without relying on OS-level threading mechanisms.

## **Project Overview**  
This library enables user-level thread management independently of the operating system, utilizing Round-Robin scheduling with fixed time slices and preemptive context switching via signals and timers (setitimer, sigaction). It supports essential thread operations such as creation, blocking, and termination, while also providing efficient sleep functionality, allowing threads to suspend themselves for a defined number of quanta. The implementation is optimized using efficient data structures, including a ready queue, a sleep map, and a min-heap for managing thread IDs.

## **Features**
- **Thread Lifecycle Management** – Create, block, resume, and terminate threads.
- **Preemptive Scheduling** – Automatic thread switching via timers.
- **Sleep Functionality** – Threads can voluntarily sleep for a defined duration.
- **Optimized Performance** – Efficient scheduling and thread ID management.
- **User-Level Implementation** – No dependency on the pthread library.
---

## **API Reference**
- **int uthread_init(int quantum_usecs)** - Initializes the thread library with a quantum size (in microseconds)
- **int uthread_spawn(thread_entry_point entry_point)** - Creates a new thread and adds it to the ready queue.  
- **int uthread_block(int tid)** - Blocks a specified thread, preventing it from executing until resumed. 
- **int uthread_resume(int tid)** - Resumes a previously blocked thread, allowing it to run again.  
- **int uthread_sleep(int num_quantums)** - Makes the currently running thread sleep for `num_quantums.
- **int uthread_terminate(int tid)** - Terminates the specified thread. If `tid == 0`, the entire process is terminated.  

## **Test Cases**  
The project includes externally written test cases that validate the correctness of the library. These tests:
- **Demonstrate API usage** in various scenarios.
- **Verify core functionalities**, including scheduling, blocking, sleeping, and termination.
- **Are located in the repository** and can be executed as described above.
- 
**Running the Tests** :
  By default, test files are commented out in the CMakeLists.txt file. To enable and run specific tests:
  1) Open CMakeLists.txt.
  2) Uncomment the desired test files under the section
