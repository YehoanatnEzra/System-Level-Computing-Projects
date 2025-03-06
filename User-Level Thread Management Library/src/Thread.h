#include <setjmp.h> // Include necessary header for handling non-local jumps

#ifndef THREAD_H
#define THREAD_H

#define STACK_SIZE 4096 /* Stack size per thread (in bytes) */
#define READY  0
#define RUNNING  1
#define BLOCKED (-1)
#define IS_AWAKE -1

typedef void (*thread_entry_point)(void); // Type definition for thread entry point function
typedef unsigned long address_t;

/**
 * @class Thread
 * @brief Represents a thread in the system.
 */
class Thread {
private:
    int id; /**< Thread ID */
    int state; /**< Thread state (READY, RUNNING, BLOCKED) */
    char* data; /**< Stack pointer for the thread */
    int quantums; /**< Number of quantums the thread has run */
    int quantumToWakeUp; /** its the thread is not sleep, whis flied will get -1;*/
    thread_entry_point entryPoint; /**< Function pointer to the thread's entry point */

    /**
   * @brief Translates an address for use with `sigsetjmp`/`siglongjmp`.
   * @param addr The address to translate.
   * @return The translated address.
   */
    address_t translate_address(address_t addr);

public:
    sigjmp_buf env; /**< Environment buffer for saving the thread's context */

    /**
     * @brief Constructor: Initializes the thread description.
     * @param entry_point Function pointer to the thread's entry point.
     * @param threadId Unique identifier for the thread.
     */
    Thread(thread_entry_point entry_point, int threadId);

    /**
     * @brief Destructor: Cleans up resources used by the thread.
     */
    ~Thread();

    /**
     * @brief Gets the thread ID.
     * @return The thread ID.
     */
    int getId() const;

    /**
     * @brief Gets the stack pointer.
     * @return The stack pointer.
     */
    char* getSP() const;

    /**
     * @brief Gets the thread state.
     * @return The thread state.
     */
    int getState() const;

    /**
     * @brief Sets the thread state.
     * @param newState The new state of the thread.
     */
    void setState(int newState);

    /**
     * @brief Increments the quantum counter.
     */
    void incQuantum();

    /**
     * @brief Gets the number of quantums the thread has run.
     * @return The number of quantums.
     */
    int getQuantum() const;

    /**
     * @brief Gets the number of quantums the thread has to wakeup.
     * @return The number of quantums.
     */
    int getQuantumToWakeUp() const;

    /**
    * @brief Sets the thread quantumToWakeUp.
    * @param newQuantumToWakeUp The new quantumToWakeUp of the thread.
    */
    void setQuantumToWakeUp(int newQuantumToWakeUp);
};

#endif // THREAD_H
