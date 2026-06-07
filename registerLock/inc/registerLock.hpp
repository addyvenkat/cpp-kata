#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Simulated Hardware Peripheral
class HarwareRegister {
public:
    bool lockBit;

    void setLock() volatile {
        lockBit = true;
    }

    void resetLock() volatile {
        lockBit = false;
    }
};

// RAII Wrapper for hardware register lock
class registerLock {
private:
    volatile HarwareRegister * register_ptr = nullptr;

public:
    
    // Constructor
    explicit registerLock(HarwareRegister * regPtr) {
        register_ptr = regPtr;
        register_ptr->setLock();
    }

    // Destructor
    ~registerLock() {
        if(register_ptr) {
            register_ptr->resetLock();
            register_ptr = nullptr;
        }
    }

    // Copy constructor and copy assignment deleted
    registerLock(const registerLock&) = delete;
    registerLock& operator=(const registerLock&) = delete;

    // Move constructor
    registerLock(registerLock&& other) noexcept {
        register_ptr = other.register_ptr;
        other.register_ptr = nullptr;
    }

    // Move assignment
    registerLock& operator=(registerLock&& other) noexcept {
        if (this != &other) {
            if (register_ptr) {
                register_ptr->resetLock();
            }
            register_ptr = other.register_ptr;
            other.register_ptr = nullptr;
        }
        return *this;
    }

};