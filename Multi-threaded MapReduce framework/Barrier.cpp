#include "Barrier.h"
#include <cstdlib>
#include <cstdio>

/**
 * @brief Constructs a Barrier for thread synchronization.
 *
 * A Barrier ensures that multiple threads wait until all have reached the same execution point.
 *
 * @param numThreads The number of threads that must reach the barrier before they proceed.
 */
Barrier::Barrier(int numThreads)
        : mutex(PTHREAD_MUTEX_INITIALIZER)
        , cv(PTHREAD_COND_INITIALIZER)
        , count(0)
        , numThreads(numThreads)
{ }

/**
 * @brief Destroys the Barrier, cleaning up mutex and condition variable resources.
 *
 * Ensures that the mutex and condition variable are properly destroyed.
 * If destruction fails, an error message is printed, and the program exits.
 */
Barrier::~Barrier()
{
    if (pthread_mutex_destroy(&mutex) != 0) {
        fprintf(stderr, "[[Barrier]] error on pthread_mutex_destroy");
        exit(1);
    }
    if (pthread_cond_destroy(&cv) != 0){
        fprintf(stderr, "[[Barrier]] error on pthread_cond_destroy");
        exit(1);
    }
}

/**
 * @brief Implements the barrier mechanism.
 *
 * Each thread that reaches this function will wait until all `numThreads` have reached it.
 * The last arriving thread will release all waiting threads.
 *
 * If any pthread function fails, an error message is printed, and the program exits.
 */
void Barrier::barrier()
{
    if (pthread_mutex_lock(&mutex) != 0){
        fprintf(stderr, "[[Barrier]] error on pthread_mutex_lock");
        exit(1);
    }
    if (++count < numThreads) {
        if (pthread_cond_wait(&cv, &mutex) != 0){
            fprintf(stderr, "[[Barrier]] error on pthread_cond_wait");
            exit(1);
        }
    } else {
        count = 0;
        if (pthread_cond_broadcast(&cv) != 0) {
            fprintf(stderr, "[[Barrier]] error on pthread_cond_broadcast");
            exit(1);
        }
    }
    if (pthread_mutex_unlock(&mutex) != 0) {
        fprintf(stderr, "[[Barrier]] error on pthread_mutex_unlock");
        exit(1);
    }
}
