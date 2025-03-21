# MapReduce Framework for Multi-Threaded Processing
This project implements a multi-threaded MapReduce framework in C++, designed for parallel data processing.
The framework allows users to define `map` and `reduce` functions and execute them efficiently using multiple threads. The implementation ensures thread safety and supports barrier synchronization.
- GitHub Actions: This project is automatically built and tested on every push using GitHub Actions, ensuring continuous integration and immediate feedback on any changes.

## Features
- **Custom MapReduce Implementation** – Uses user-defined `map` and `reduce` functions.
- **Multi-threading Support** – Leverages the `pthread` library for concurrent execution.
- **Barrier Synchronization** – Implements a reusable barrier to synchronize threads.
- **Flexible Job Management** – Supports job state tracking, thread synchronization, and workload distribution.

## Project Structure
- **Barrier.h**                     - Header file for thread barrier
- **Barrier.cpp**                   - Implementation of thread barrier
- **MapReduceClient.h**             - Abstract interface for defining MapReduce jobs
- **MapReduceFramework.h**          - Header file defining the MapReduce framework
- **MapReduceFramework.cpp**        - Implementation of the MapReduce framework
- **test1-1_thread_1_process.cpp**  - Test with 1 thread and 1 process
- **test4-1_thread_4_process.cpp**  - Test with 4 processes using 1 thread each


## How to Use
- Implement a custom class inheriting from `MapReduceClient` (You can find usage examples in the provided test files), defining:
   - `map` function – Processes input data and emits intermediate key-value pairs.
   - `reduce` function – Aggregates values corresponding to each key.
- Create input data as a vector of (K1*, V1*) pairs.
- Start the MapReduce job using:
   ```cpp
   JobHandle job = startMapReduceJob(client, inputVec, outputVec, numThreads);
   ```
- Wait for job completion using:
   ```cpp
   waitForJob(job);
   ```
- Retrieve results from `outputVec`.

## Tests
This repository includes pre-written test cases (`test1-1_thread_1_process.cpp` and `test4-1_thread_4_process.cpp`) to validate the correctness and performance of the framework. These tests were not written by myself but can be used to evaluate thread safety and execution behavior.
The test files are already included in the CMake setup but are commented out by default in `CMakeLists.txt`, to enable and run a test, simply uncomment the relevant test file in CMakeLists.txt before building.

### GitHub actions
I created a GitHub actions workflow that automatically compiles the code and runs the tests on every push. the workflow:
- Compiles the framwork and test executables.
- Runs each test and produce an output file
- Compares the output agains an expected file to verify correctness. if any difference is found, the build fails, ensuring that any regressions or errors are caught immediately.

to see the workflow configuration, check the .github/workflows directory in this repository.



