# MapReduce Framework for Multi-Threaded Processing

## Overview
This project implements a multi-threaded MapReduce framework in C++, designed for parallel data processing.
The framework allows users to define **map** and **reduce** functions and execute them efficiently using multiple threads. The implementation ensures thread safety and supports barrier synchronization.

## Features
- **Custom MapReduce Implementation** – Uses user-defined `map` and `reduce` functions.
- **Multi-threading Support** – Leverages the `pthread` library for concurrent execution.
- **Barrier Synchronization** – Implements a reusable barrier to synchronize threads.
- **Flexible Job Management** – Supports job state tracking, thread synchronization, and workload distribution.

## Project Structure
```
Barrier.h                     # Header file for thread barrier
Barrier.cpp                   # Implementation of thread barrier
MapReduceClient.h             # Abstract interface for defining MapReduce jobs
MapReduceFramework.h          # Header file defining the MapReduce framework
MapReduceFramework.cpp        # Implementation of the MapReduce framework
test1-1_thread_1_process.cpp  # Test with 1 thread and 1 process
test4-1_thread_4_process.cpp  # Test with 4 processes using 1 thread each
```

## How to Use
1. Implement a custom class inheriting from `MapReduceClient` (You can find usage examples in the provided test files), defining:
   - `map` function – Processes input data and emits intermediate key-value pairs.
   - `reduce` function – Aggregates values corresponding to each key.
2. Create input data as a vector of `(K1*, V1*)` pairs.
3. Start the MapReduce job using:
   ```cpp
   JobHandle job = startMapReduceJob(client, inputVec, outputVec, numThreads);
   ```
4. Wait for job completion using:
   ```cpp
   waitForJob(job);
   ```
5. Retrieve results from `outputVec`.

## Tests
This repository includes pre-written test cases (`test1-1_thread_1_process.cpp` and `test4-1_thread_4_process.cpp`) to validate the correctness and performance of the framework.
These tests were not written by myself but can be used to evaluate thread safety and execution behavior.

## Running the Project

This project includes a pre-configured CMake setup, allowing easy compilation and testing.

### Test Execution

The test files (`test1-1_thread_1_process.cpp`, `test4-1_thread_4_process.cpp`) are already included in the CMake setup but **are commented out by default** in `CMakeLists.txt`.

To enable and run a test, simply uncomment the relevant test file in `CMakeLists.txt` before building.



