# **Hierarchical Virtual Memory Management System**
This project implements a virtual memory management system in C++ using hierarchical page tables.
The system translates virtual addresses to physical addresses and efficiently manages page allocation, swapping, and eviction. 
The project was developed as part of an Operating Systems course and demonstrates low-level memory management techniques, including:
- Hierarchical Page Tables for efficient memory translation.
- Page Fault Handling & Eviction to manage memory constraints.
- No Dynamic Memory Allocation, enforcing strict memory usage constraints.
- Custom Page Replacement Algorithm based on Cyclical Distance.

## **Features**
- **Virtual to Physical Address Translation** – Hierarchical traversal of page tables to resolve addresses.  
- **Page Fault Handling** – Pages are swapped in and out dynamically when needed.  
- **Custom Page Replacement Strategy** – Finds an optimal frame to evict based on memory access patterns.  
- **Zero Dynamic Allocation** – The entire system works using only the provided physical memory simulation.  
- **Efficient Memory Search with DFS** – Implements a DFS algorithm to find available memory slots and identify pages for eviction.  
- **Test Cases Included** – Pre-written test case (`SimpleTest.cpp`) to validate correctness.  

## **How It Works**
This project simulates a **virtual memory system** using a hierarchical page table structure. It operates in the following manner:
- **Memory Initialization** – The root page table is stored in frame 0, and the memory is divided into fixed-size pages and frames.
- **Address Translation** – Virtual addresses are translated step-by-step through a hierarchical structure of tables, mapping virtual pages to physical frames.
- **Page Fault Handling** – If a requested page is not in physical memory, the system either allocates a new frame or evicts an existing page to free up space.
- **Page Swapping & Eviction** – When memory is full, the system selects a page for eviction using a cyclical distance algorithm, ensuring minimal disruption to frequently accessed pages.

## **Project Structure**
- **VirtualMemory.h** -           API for virtual memory management
- **VirtualMemory.cpp**        - Implementation of virtual memory functions
- **PhysicalMemory.h**         - API for simulated physical memory
- **PhysicalMemory.cpp**       - Simulated physical memory with page read/write
- **MemoryConstants.h**        - Constants for memory sizes and configurations
- **SimpleTest.cpp**           - Test case (not written by me)

## Tests

This repository includes pre-written test cases (Simple.Test.cpp). These tests were not written by me, but they can be used to evaluate thread safety and execution behavior.
The test file is already included in the CMake setup but is commented out by default in CMakeLists.txt. To enable and run a test, simply uncomment the relevant test file in CMakeLists.txt before building.



