# User-Level Thread Management Library
This project is a **user-level thread management library** for a Linux-based operating system. It provides efficient **thread management**, including creation, scheduling, and termination, without relying on OS-level threading mechanisms.

##  Project Description  
This library provides user-level thread management, operating independently of the OS threading mechanisms. It supports creating, running, suspending, blocking, and terminating threads while utilizing context switching and time management mechanisms. The project implements a Round-Robin scheduler with a fixed quantum per thread and includes a sleep mechanism, allowing threads to enter a waiting state and wake up after a defined number of quantums. To optimize performance, the library employs efficient data structures, including a ready queue, a sleep map for sleeping threads, and a min-heap for managing available thread IDs. Additionally, it leverages timers and signals (setitimer, sigaction) for precise thread scheduling and context switching using (sigsetjmp/siglongjmp).

##  Main Features  
- **Thread Lifecycle Management:** Create, block, resume, and terminate threads.  
- **Round-Robin Scheduling:** Threads are scheduled in a cyclic manner with a fixed quantum.  
- **Sleep Functionality:** Threads can voluntarily sleep for a specified number of quantums.  
- **Preemptive Scheduling:** The library automatically switches between threads using timer-based signals.  
- **User-Level Implementation:** No reliance on the Linux `pthread` library.  

##  Included Test Cases  

This project includes **several test cases** that verify the correctness of the library. These tests:
- **Demonstrate how to use the API** in different scenarios.
- **Validate core functionality**, such as scheduling, sleeping, blocking, and termination.
- Were written externally, but provide an excellent reference for using the library.


##  API Reference  

### **Initialize the Thread System**

int uthread_init(int quantum_usecs);

- Initializes the thread library with a **quantum size** (in microseconds).  
- Must be called **before** any other `uthread_` function.  

### **Create a New Thread**

int uthread_spawn(thread_entry_point entry_point);

- Creates a new thread and assigns it to the **ready queue**.  

### **Block a Thread**

int uthread_block(int tid);
```
- Blocks the specified thread, preventing it from running until resumed.  

### **Resume a Blocked Thread**
```cpp
int uthread_resume(int tid);

- Resumes a previously blocked thread, allowing it to run again.  

### **Put a Thread to Sleep**
```
int uthread_sleep(int num_quantums);
```
- Makes the **currently running thread sleep** for `num_quantums`.  

### **Terminate a Thread**
```cpp
int uthread_terminate(int tid);
```
- Terminates the specified thread.  
- If `tid == 0`, the entire process is terminated.  


