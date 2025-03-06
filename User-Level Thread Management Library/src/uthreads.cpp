#include "uthreads.h"
#include "Thread.h"
#include <vector>
#include <csignal>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <list>
#include <sys/time.h>

// Error messages
#define THREAD_ERROR "thread library error: "
#define MAX_THREAD_ERROR "maximum threads amount reached"
#define NEGATIVE_QUANTUM "negative quantum is not allowed"
#define SIGACTION_ERROR_MEG "failed to set sigaction"
#define SYSTEM_CALL_ERR_MES "system error: failed in allocated memory"

// Constants and states
#define MICROSECONDS_IN_SECOND 1000000
#define FAILED (-1)
#define SUCCEED 0
#define BLOCK_OR_SLEEP_CUR 1
#define SLEEP 2
#define TERMINATE_CUR 2
#define INIT_CUR 0
#define LONG_JMP 1
#define SET_JMP 0
#define IS_AWAKE -1

// Global variables
int curMaxId = 0;
int quantum = 0;
int totalQuantums = 0;
int curThreadID = 0;
int thread_count = 0;
sigset_t cmaskSet;
struct itimerval timer;

// Comparator for Min Heap
struct MinHeapComparator {
    bool operator()(int left, int right) {
        return left > right;
    }
};

// Data structures
std::priority_queue<int, std::vector<int>, MinHeapComparator> removedIdsMinHeap;
std::list<Thread *> readyQueue;
std::unordered_map<unsigned int, std::vector<int>> sleepMap; // time and ids
Thread *thread_log[MAX_THREAD_NUM];


/**
     * Namespace containing utility functions for managing virtual timers and resources.
     * This namespace includes functions for setting virtual timers, stopping timers, freeing resources, updating
     * sleep's map and a placeholder function to the main thread.
     */
namespace {
    /**
    * Sets a virtual timer with the specified quantum value.
    * @param quantum The quantum value in microseconds.
    * @return SUCCEED if the timer is successfully set, FAILED otherwise.
    */
    unsigned int set_alarm(int quantum) {
        sigprocmask(SIG_BLOCK, &cmaskSet, nullptr);
        timer.it_interval.tv_usec = quantum % MICROSECONDS_IN_SECOND;
        timer.it_interval.tv_sec = quantum / MICROSECONDS_IN_SECOND;
        timer.it_value.tv_usec = quantum % MICROSECONDS_IN_SECOND;
        timer.it_value.tv_sec = quantum / MICROSECONDS_IN_SECOND;
        if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
            perror("setitimer");
            sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
            return FAILED;
        }
        sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
        return SUCCEED;
    }

    /**
    * Stops the virtual timer.
    */
    void stop_timer() {
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = 0;
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 0;
        if (setitimer(ITIMER_REAL, &timer, nullptr) == -1) {
            perror("setitimer");
        }
    }

    /**
    * Frees resources and exits the program with the specified exit code.
    * @param exitCode The exit code to be used when exiting the program.
    */
    void freeResourcesAndExit(int exitCode) {
        readyQueue.clear();
        for (Thread *t: thread_log) {
            delete (t);
            t = nullptr;
        }
        exit(exitCode);
    }

    /**
 * This function checks if there are threads in the sleep state with the specified number
 * of quantums until wakeup. If found, it changes their state to READY and adds them to the
 * ready queue for execution.
 *
 * @param quantums The number of quantums to check for threads to wake up.
 */
    void updateSleep(int quantums) {
        if (sleepMap.find(quantums) != sleepMap.end()) {
            for (int tid: sleepMap[quantums]) {
                if (thread_log[tid] && thread_log[tid]->getState() == SLEEP &&
                    quantums == thread_log[tid]->getQuantumToWakeUp()) {
                    thread_log[tid]->setState(READY);
                    readyQueue.push_back(thread_log[tid]);
                }
            }
            sleepMap.erase(quantums);
        }
    }

    /**
     * Placeholder function.
     */
    void empty(void) {}
}


void scheduler(int action) {
    sigprocmask(SIG_BLOCK, &cmaskSet, nullptr);
    stop_timer();
    updateSleep(totalQuantums);
    // In case we need to push back the current thread
    if (action != TERMINATE_CUR) {
        int stateOfJump = sigsetjmp(readyQueue.front()->env, 1);
        if (stateOfJump == SET_JMP && thread_count < 2) {
            set_alarm(quantum);
            totalQuantums += 1;
            readyQueue.front()->incQuantum();
            sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
            return;
        } else if (stateOfJump == LONG_JMP) { // Jumping from siglongjmp would end in returning to the Thread function
            set_alarm(quantum);
            sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
            return;
        }
        if (action != BLOCK_OR_SLEEP_CUR) {
            readyQueue.push_back(readyQueue.front());
        }
        readyQueue.pop_front();
    }
    totalQuantums += 1;
    readyQueue.front()->incQuantum();
    readyQueue.front()->setState(RUNNING);
    set_alarm(quantum);
    // deleting and freeing the running thread only after the scheduler job is finished
    if (action == TERMINATE_CUR) {
        int log = curThreadID;
        curThreadID = readyQueue.front()->getId();
        Thread* t = thread_log[log];
        thread_log[log] = nullptr;
        delete(t);
    } else {
        curThreadID = readyQueue.front()->getId();
    }
    sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
    siglongjmp(readyQueue.front()->env, 1);
}

// Initialization function
int uthread_init(int quantum_usecs) {
    if (quantum_usecs <= 0) {
        std::cerr << THREAD_ERROR << NEGATIVE_QUANTUM << std::endl;
        return FAILED;
    }
    sigaddset(&cmaskSet, SIGVTALRM); //init set that inclides only the alarm's signal
    quantum = quantum_usecs;
    uthread_spawn(&empty);

    if (sigsetjmp(thread_log[0]->env, 1) != 0) {
        return FAILED;
    } else {
        thread_log[0]->setState(RUNNING);
        struct sigaction s = {0};
        s.sa_handler = &(scheduler);

        if (sigaction(SIGVTALRM, &s, NULL) == FAILED) {
            std::cerr << SIGACTION_ERROR_MEG << std::endl;
            return FAILED;
        }
        scheduler(INIT_CUR);
        return SUCCEED;
    }
}


int uthread_spawn(thread_entry_point entry_point) {
    sigprocmask(SIG_BLOCK, &cmaskSet, nullptr);
    if (thread_count >= MAX_THREAD_NUM || entry_point == nullptr) {
        std::cerr << THREAD_ERROR << MAX_THREAD_ERROR << std::endl;
        sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
        return FAILED;
    }
    // generating ID
    int id;
    if (removedIdsMinHeap.empty()) {
        id = curMaxId;
        curMaxId += 1;
    } else {
        id = removedIdsMinHeap.top();
        removedIdsMinHeap.pop();
    }
    try {
        auto *thread = new Thread(entry_point, id);
        readyQueue.push_back(thread);
        thread_log[id] = thread;
        thread_count++;
    } catch (const std::bad_alloc &e) {
        std::cerr << SYSTEM_CALL_ERR_MES << id << std::endl;
        freeResourcesAndExit(1); //free the resource and exit with exit code 1
    }
    sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
    return id;
}

int uthread_terminate(int tid) {
    sigprocmask(SIG_BLOCK, &cmaskSet, nullptr);
    stop_timer();
    // ID check
    bool curThreadTerminate = tid == curThreadID;
    if (thread_log[tid] == nullptr || tid < 0) {
        sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
        return FAILED;
    } else if (tid == 0) {
        freeResourcesAndExit(0);
    }
    // Removing the thread from database and freeing it's space
    removedIdsMinHeap.push(tid);
    readyQueue.remove(thread_log[tid]);
    // Releasing the memory of non running thread is ok
    thread_count--;
    if (!curThreadTerminate) {
        delete (thread_log[tid]);
        thread_log[tid] = nullptr;
    } else {
        scheduler(TERMINATE_CUR);
    }
    sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
    return SUCCEED;
}

int uthread_block(int tid) {
    sigprocmask(SIG_BLOCK, &cmaskSet, nullptr);
    if (tid == 0 || thread_log[tid] == nullptr) {
        sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
        return FAILED;
    }
    if (thread_log[tid]->getState() == BLOCKED) {
        sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
        return SUCCEED;
    }
    thread_log[tid]->setState(BLOCKED);
    // In case the current thread is being blocked
    if (tid == curThreadID) {
        scheduler(BLOCK_OR_SLEEP_CUR);
    } else {
        readyQueue.remove(thread_log[tid]);
    }
    sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
    return SUCCEED;
}


int uthread_resume(int tid) {
    sigprocmask(SIG_BLOCK, &cmaskSet, nullptr);

    if (tid < 0 || thread_log[tid] == nullptr) {
        sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
        return FAILED;
    }
    if (thread_log[tid]->getState() == RUNNING || thread_log[tid]->getState() == READY) {
        sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
        return SUCCEED;
    }
    thread_log[tid]->setState(READY);
    readyQueue.push_back(thread_log[tid]);
    sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
    return SUCCEED;
}

int uthread_sleep(int num_quantums) {
    sigprocmask(SIG_BLOCK, &cmaskSet, nullptr);
    if (curThreadID == 0) {
        sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
        return FAILED;
    }
    int quantumToWakeUp = totalQuantums + num_quantums;
    if (sleepMap.find(quantumToWakeUp) != sleepMap.end()) {
        sleepMap[quantumToWakeUp].push_back(curThreadID);
    } else {
        sleepMap[quantumToWakeUp] = {curThreadID};
    }
    thread_log[curThreadID]->setQuantumToWakeUp(quantumToWakeUp);
    thread_log[curThreadID]->setState(SLEEP);

    scheduler(BLOCK_OR_SLEEP_CUR);

    sigprocmask(SIG_UNBLOCK, &cmaskSet, nullptr);
    return SUCCEED;
}

int uthread_get_tid() {
    return curThreadID;
}

int uthread_get_total_quantums() {
    return totalQuantums;
}

int uthread_get_quantums(int tid) {
    if (thread_log[tid] == nullptr) {
        return FAILED;
    }
    return thread_log[tid]->getQuantum();
}
