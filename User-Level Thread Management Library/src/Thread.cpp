#include "Thread.h"
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <iostream>


#define JB_SP 6
#define JB_PC 7

address_t Thread::translate_address(address_t addr) {
    address_t ret;
    asm volatile("xor    %%fs:0x30,%0\n"
                 "rol    $0x11,%0\n"
            : "=g" (ret)
            : "0" (addr));
    return ret;
}

// Constructor: Initialize the thread description
Thread::Thread(thread_entry_point entry_point, int threadId) {
    id = threadId;
    entryPoint = entry_point;
    state = READY;
    quantumToWakeUp = IS_AWAKE;
    quantums=0;
    try {
        data = new char[STACK_SIZE];
    } catch (const std::bad_alloc& e) {
        throw;
    }

    address_t sp = (address_t) data + STACK_SIZE - sizeof(address_t);
    address_t pc = (address_t) entry_point;

    sigsetjmp(env, 1);
    (env->__jmpbuf)[JB_SP] = translate_address(sp);
    (env->__jmpbuf)[JB_PC] = translate_address(pc);
    sigemptyset(&(env->__saved_mask));
}

// Destructor
Thread::~Thread() {
    delete[] data;
}

// Getter methods
int Thread::getId() const {
    return id;
}

char *Thread::getSP() const {
    return data;
}

int Thread::getState() const {
    return state;
}

int Thread::getQuantumToWakeUp() const {
    return quantumToWakeUp;
}

int Thread::getQuantum() const {
    return quantums;
}

// setter methods
void Thread::setState(int newState) {
    state = newState;
}
void Thread::setQuantumToWakeUp(int newQuantumToWakeUp)  {
    quantumToWakeUp=newQuantumToWakeUp;
}

//other methods
void Thread::incQuantum() {
    quantums++;
}


