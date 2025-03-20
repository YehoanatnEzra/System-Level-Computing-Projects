#include "MapReduceFramework.h"
#include <pthread.h>
#include <vector>
#include <atomic>
#include <algorithm>
#include <iostream>
#include <map>
#include "Barrier.h"

/**
 *
 *
 * System error messages for debugging and error handling.
 */
#define SYS_ERR std::cout << "system error: "
#define MUTEX_LOCK_SYS_ERR SYS_ERR << "failed to lock mutex" << std::endl
#define MUTEX_UNLOCK_SYS_ERR SYS_ERR << "failed to unlock mutex" << std::endl
#define THREADS_CREATION_SYS_ERR SYS_ERR << "failed to create threads " << std::endl

/**
 * Thread and mutex-related constants.
 */
#define MAIN_THREAD_ID 0   // Identifier for the main thread
#define MUTEXES 10         // Total number of mutexes used in the system

/**
 * Mutex indices for different access regions in the framework.
 */
#define INIT_STAGE_ACCESS 1       // Mutex for accessing the initialization stage
#define MAP_VECTOR_ACCESS 2       // Mutex for accessing the map vector
#define EMIT2_ACCESS 3            // Mutex for emitting intermediate key-value pairs
#define REDUCE_VECTOR_ACCESS 5    // Mutex for accessing the reduce vector
#define EMIT3_ACCESS 7            // Mutex for emitting final key-value pairs
#define REDUCE_STATE_ACCESS 8     // Mutex for tracking the reduce stage state
#define MAP_STATE_ACCESS 9        // Mutex for tracking the map stage state

typedef std::vector<IntermediateVec *> IntermediateDB;

struct JobContext {
    const MapReduceClient &client; // the client with the functions map and reduce
    const InputVec &inputVec; // vector of input pairs K1,K2
    pthread_t *threads; // all the threads used in the map reduce sequence
    std::vector<pthread_mutex_t *>* mutexes; // all the mutexes used in the map reduce sequence
    JobState *jobState;
    Barrier *barrier;
    IntermediateDB &mappedVectorDB; // mapped db for emit2 output
    IntermediateDB &shuffledVectorDB; // shuffled db for shuffle function output
    OutputVec *outputVec; // emit3 output for
    std::map<pthread_t, int> *mappingJobToThread; // map for threads and their allocated microjob of mapping
    pthread_cond_t *shuffleCondition; // cv used for shuffle
    std::atomic<int> *finishedMappingJobs;
    std::atomic<int> *finishedReducingJobs;
    std::atomic<int> *nextMapVec;
    std::atomic<int> *nextReduceVec;
    float *shuffledDbSize;
    bool waitFlag = false;

    JobContext(OutputVec *outputVec, JobState *jobState, const MapReduceClient &client,
               const InputVec &inputVec, IntermediateDB &mappedVectorDB, IntermediateDB &shuffledVectorDB,
               pthread_t *threads, Barrier *barrier,
               std::map<pthread_t, int> *mappingJobToThread, pthread_cond_t *shuffleCondition, float* shuffledDbSize)
            : client(client), inputVec(inputVec), mappedVectorDB(mappedVectorDB), shuffledVectorDB(shuffledVectorDB),
              outputVec(outputVec), jobState(jobState), threads(threads), barrier(barrier),
              mappingJobToThread(mappingJobToThread), shuffleCondition(shuffleCondition),
              finishedMappingJobs(new std::atomic<int>(0)),
              finishedReducingJobs(new std::atomic<int>(0)),
              nextMapVec(new std::atomic<int>(0)),
              nextReduceVec(new std::atomic<int>(0)), shuffledDbSize(shuffledDbSize) {

            mutexes = new std::vector<pthread_mutex_t*>;
            for (int i = 0; i < MUTEXES; i++) {
                auto* m = new pthread_mutex_t;
                pthread_mutex_init(m, nullptr);
                mutexes->push_back(m);
            }
    }

    // Destructor to release allocated memory
    ~JobContext() {
        delete finishedMappingJobs;
        delete finishedReducingJobs;
        delete nextMapVec;
        delete nextReduceVec;
        delete mappingJobToThread;
        delete shuffleCondition;
        delete barrier;
        delete[] threads;

        for(int i =0 ; i < mutexes->size(); i++){
            pthread_mutex_destroy(mutexes->at(i));

        }
        delete jobState;

        for (auto &vec : mappedVectorDB) {
            delete vec;
        }
        for (auto &vec : shuffledVectorDB) {
            delete vec;
        }
    }
};

/**
 * Locks the given mutex, exits on failure.
 */
void lock(pthread_mutex_t *mutexToLock) {
    if (pthread_mutex_lock(mutexToLock) != 0) {
        MUTEX_LOCK_SYS_ERR;
        exit(EXIT_FAILURE);
    }
}

/**
 * Unlocks the given mutex, exits on failure.
 */
void unlock(pthread_mutex_t *mutex) {
    if (pthread_mutex_unlock(mutex) != 0) {
        MUTEX_UNLOCK_SYS_ERR;
        exit(EXIT_FAILURE);
    }
}

/**
 * Updates the job state with the given stage and percentage.
 */
void setState(JobState *state, stage_t newStage, float percentage) {
    state->stage = newStage;
    state->percentage = percentage;
}

/**
 * This file implements the core logic for the MapReduce framework,
 * including mapping, shuffling, and reducing operations.
 */
float sizeIntermediateDB(IntermediateDB &vector) {
    float size = 0;
    for (auto vec: vector) {
        size += vec->size();
    }
    return size;
}

/**
 * Performs the shuffle phase by grouping intermediate values based on keys.
 * Extracts the maximum key from the mapped database and groups matching values.
 */
void shuffle(IntermediateDB &mappedVectorDB, IntermediateDB &shuffledVectorDB, JobState *jobState) {
    float size = sizeIntermediateDB(mappedVectorDB);
    float finishedShufflingJobs = 0;

    while (!mappedVectorDB.empty()) {
        // Find the maximum key in the mapped database
        auto maxKey = mappedVectorDB.front()->back().first;
        for (auto vec: mappedVectorDB) {
            if (!vec->empty() && (maxKey->operator<(*vec->back().first))) {
                maxKey = vec->back().first;
            }
        }

        // Collect all values with the same key
        auto newVec = new IntermediateVec;
        for (int j = 0; j < mappedVectorDB.size(); j++) {
            auto vec = mappedVectorDB[j];

            while (!vec->empty() && (!vec->back().first->operator<(*maxKey)) &&
                   (!(maxKey->operator<(*vec->back().first)))) {
                newVec->push_back(vec->back());
                vec->pop_back();
                finishedShufflingJobs++;
            }
        }

        // Remove empty vectors from the mapped database
        for (auto it = mappedVectorDB.begin(); it != mappedVectorDB.end(); ) {
            if ((*it)->empty()) {
                it = mappedVectorDB.erase(it);
            } else {
                ++it;
            }
        }

        shuffledVectorDB.push_back(newVec);
        setState(jobState, SHUFFLE_STAGE, (finishedShufflingJobs / size) * 100);
    }
}

void emit2(K2 *key, V2 *value, void *context) {
    auto jobContext = static_cast<JobContext *>(context);
    auto db = jobContext->mappedVectorDB;

    lock((*(jobContext->mutexes))[EMIT2_ACCESS]);
    int curJobIndex = jobContext->mappingJobToThread->find(pthread_self())->second;
    db[curJobIndex]->push_back(std::make_pair(key, value));
    unlock((*(jobContext->mutexes))[EMIT2_ACCESS]);
}

void emit3(K3 *key, V3 *value, void *context) {
    auto jobContext = static_cast<JobContext *>(context);
    lock((*(jobContext->mutexes))[EMIT3_ACCESS]);

    auto *outputVec = static_cast<OutputVec *>(jobContext->outputVec);
    outputVec->push_back(std::make_pair(key, value));

    unlock((*(jobContext->mutexes))[EMIT3_ACCESS]);
}

/**
 * Main thread function that manages the Map, Shuffle, and Reduce stages.
 */
void *threadManager(void *arg) {
    auto *jobContext = static_cast<JobContext *>(arg);
    IntermediateDB &mappedVectorDB = jobContext->mappedVectorDB;
    IntermediateDB &shuffledVectorDB = jobContext->shuffledVectorDB;

    // Initialize the Map phase
    lock((*(jobContext->mutexes))[INIT_STAGE_ACCESS]);
    if (jobContext->jobState->stage == UNDEFINED_STAGE) {
        setState(jobContext->jobState, MAP_STAGE, 0);
    }
    unlock((*(jobContext->mutexes))[INIT_STAGE_ACCESS]);

    // Map Stage: Assign tasks to available threads
    while (jobContext->nextMapVec->load() < jobContext->inputVec.size()) {
        lock((*(jobContext->mutexes))[MAP_VECTOR_ACCESS]);
        int partialJobIndex = jobContext->nextMapVec->load();
        jobContext->nextMapVec->fetch_add(1);
        unlock((*(jobContext->mutexes))[MAP_VECTOR_ACCESS]);

        if (jobContext->mappingJobToThread->find(pthread_self()) != jobContext->mappingJobToThread->end()) {
            jobContext->mappingJobToThread->erase(pthread_self());
        }
        jobContext->mappingJobToThread->insert({pthread_self(), partialJobIndex});

        auto &curVec = jobContext->inputVec[partialJobIndex];
        jobContext->client.map(curVec.first, curVec.second, jobContext);
        std::sort(mappedVectorDB[partialJobIndex]->begin(), mappedVectorDB[partialJobIndex]->end());
        jobContext->finishedMappingJobs->fetch_add(1);

        lock((*(jobContext->mutexes))[MAP_STATE_ACCESS]);
        setState(jobContext->jobState, MAP_STAGE,
                 ((float)jobContext->finishedMappingJobs->load() / (float)jobContext->inputVec.size()) * 100);
        unlock((*(jobContext->mutexes))[MAP_STATE_ACCESS]);
    }

    jobContext->barrier->barrier();

    // Shuffle Stage: Main thread performs the shuffle
    if (jobContext->threads[MAIN_THREAD_ID] == pthread_self()) {
        setState(jobContext->jobState, SHUFFLE_STAGE, 0);
        shuffle(jobContext->mappedVectorDB, shuffledVectorDB, jobContext->jobState);
        *(jobContext->shuffledDbSize) = shuffledVectorDB.size();
        pthread_cond_broadcast(jobContext->shuffleCondition);
    }

    // Reduce Stage: Process the shuffled data
    if (jobContext->jobState->stage == SHUFFLE_STAGE) {
        setState(jobContext->jobState, REDUCE_STAGE, 0);
    }

    while (shuffledVectorDB.size() > jobContext->nextReduceVec->load()) {
        lock((*(jobContext->mutexes))[REDUCE_VECTOR_ACCESS]);
        auto *curShuffledVec = shuffledVectorDB.back();
        shuffledVectorDB.pop_back();
        unlock((*(jobContext->mutexes))[REDUCE_VECTOR_ACCESS]);

        jobContext->client.reduce(curShuffledVec, jobContext);
        jobContext->finishedReducingJobs->fetch_add(1);

        lock((*(jobContext->mutexes))[REDUCE_STATE_ACCESS]);
        setState(jobContext->jobState, REDUCE_STAGE,
                 ((float)jobContext->finishedReducingJobs->load() / (float)*(jobContext->shuffledDbSize)) * 100);
        unlock((*(jobContext->mutexes))[REDUCE_STATE_ACCESS]);
    }
    return nullptr;
}

JobHandle startMapReduceJob(const MapReduceClient &client, const InputVec &inputVec, OutputVec &outputVec,
                            int multiThreadLevel) {
    // Allocations
    auto mappedVectorDB = new std::vector<IntermediateVec *>(inputVec.size());
    for (int i = 0; i < inputVec.size(); i++) {
        mappedVectorDB->at(i) = new IntermediateVec();
    }

    auto shuffledVectorDB = new std::vector<IntermediateVec *>;
    auto threads = (pthread_t *) malloc(sizeof(pthread_t) * (multiThreadLevel));
    auto barrier = new Barrier(multiThreadLevel);
    auto jobState = new JobState{UNDEFINED_STAGE, 0};
    auto mappingJobToThread = new std::map<pthread_t, int>; // Maps each thread to its corresponding intermediate vector index
    auto shuffleCondition = new pthread_cond_t;
    auto shuffledDbSize = new float;

    auto jobContext = new JobContext(&outputVec, jobState, client,
                                     inputVec, *mappedVectorDB, *shuffledVectorDB,
                                     threads, barrier, mappingJobToThread, shuffleCondition, shuffledDbSize);

    // Initializing condition variable
    pthread_cond_init(shuffleCondition, NULL);

    // Creating worker threads
    for (int i = 0; i < multiThreadLevel; ++i) {
        if (pthread_create(&threads[i], nullptr, threadManager, jobContext) != 0) {
            THREADS_CREATION_SYS_ERR;
            exit(EXIT_FAILURE);
        }
    }
    return static_cast<JobHandle>(jobContext);
}

void getJobState(JobHandle job, JobState *state) {
    JobContext *jobContext = static_cast<JobContext *>(job);
    *state = *(jobContext->jobState);
}

void closeJobHandle(JobHandle job) {
    // Ensures resources are not freed before the job completes
    waitForJob(job);
    delete static_cast<JobContext *>(job);
}

/**
 * Blocks the calling thread until the given job completes.
 */
void waitForJob(JobHandle job) {
    JobContext *jobContext = static_cast<JobContext *>(job);

    if (jobContext->waitFlag) {
        return;
    } else {
        jobContext->waitFlag = true;
    }

    // Wait for all worker threads to complete
    for (int i = 0; i < sizeof(jobContext->threads) / sizeof(pthread_t); ++i) {
        pthread_join(jobContext->threads[i], nullptr);
    }
}

