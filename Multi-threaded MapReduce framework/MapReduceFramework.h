#ifndef MAPREDUCEFRAMEWORK_H
#define MAPREDUCEFRAMEWORK_H

#include "MapReduceClient.h"

/**
 * @brief Handle to a running MapReduce job.
 * Used to manage and track job execution.
 */
typedef void* JobHandle;

/**
 * @brief Enumeration of possible stages in the MapReduce job lifecycle.
 */
enum stage_t {
    UNDEFINED_STAGE=0,  ///< The job has not started yet.
    MAP_STAGE=1,        ///< The job is currently executing the Map phase.
    SHUFFLE_STAGE=2,    ///< The job is in the Shuffle phase.
    REDUCE_STAGE=3      ///< The job is in the Reduce phase.
};

/**
 * @brief Represents the current state of a MapReduce job.
 */
typedef struct {
    stage_t stage;    ///< The current stage of the job.
    float percentage; ///< The progress percentage of the current stage.
} JobState;

/**
 * @brief Emits an intermediate key-value pair during the Map phase.
 *
 * @param key The intermediate key (K2*).
 * @param value The intermediate value (V2*).
 * @param context The execution context containing job-specific data.
 */
void emit2 (K2* key, V2* value, void* context);

/**
 * @brief Emits a final key-value pair during the Reduce phase.
 *
 * @param key The final key (K3*).
 * @param value The final value (V3*).
 * @param context The execution context containing job-specific data.
 */
void emit3 (K3* key, V3* value, void* context);

/**
 * @brief Starts a new MapReduce job with the given input and parameters.
 *
 * @param client The MapReduceClient instance implementing the map and reduce logic.
 * @param inputVec The input vector containing key-value pairs (K1, V1).
 * @param outputVec The output vector that will store results (K3, V3).
 * @param multiThreadLevel The number of worker threads to use.
 * @return A JobHandle that can be used to track and manage the job.
 */
JobHandle startMapReduceJob(const MapReduceClient& client,
                            const InputVec& inputVec, OutputVec& outputVec,
                            int multiThreadLevel);

/**
 * @brief Waits for the given MapReduce job to complete.
 *
 * @param job The JobHandle representing the running job.
 */
void waitForJob(JobHandle job);

/**
 * @brief Retrieves the current state of the given MapReduce job.
 *
 * @param job The JobHandle representing the running job.
 * @param state A pointer to a JobState structure that will be updated with the job's current state.
 */
void getJobState(JobHandle job, JobState* state);

/**
 * @brief Cleans up resources associated with the given job and releases its handle.
 *
 * @param job The JobHandle representing the job to be closed.
 */
void closeJobHandle(JobHandle job);

#endif //MAPREDUCEFRAMEWORK_H
