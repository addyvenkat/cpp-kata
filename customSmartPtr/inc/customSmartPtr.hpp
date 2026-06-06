#pragma once 

#include <stdint.h>
#include <stdlib.h>

template <typename T>
class customSmartPtr {
private:
    T* ptr = nullptr;
    size_t* ref_count = nullptr;

    void cleanup() {
        if (ref_count) {
            (*ref_count)--;
            if (*ref_count == 0) {
                delete ptr;
                delete ref_count;
            }
        }
    }

public:
    // Basic Constructor
    explicit customSmartPtr(T* p = nullptr) {
        ptr = p;
        if (ptr) {
            ref_count = new size_t(1);
        }
    }

    // Copy Constructor
    customSmartPtr(const customSmartPtr& other) {
        ptr = other.ptr;
        ref_count = other.ref_count;
        if (ref_count) {
            (*ref_count)++;
        }
    }

    // Move Constructor
    customSmartPtr(customSmartPtr&& other) noexcept {
        ptr = other.ptr;
        ref_count = other.ref_count;
        other.ptr = nullptr;
        other.ref_count = nullptr;
    }

    // Copy Assignment Operator
    customSmartPtr& operator=(const customSmartPtr& other) {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            ref_count = other.ref_count;
            if (ref_count) {
                (*ref_count)++;
            }
        }
        return *this;
    }

    // Move Assignment Operator
    customSmartPtr& operator=(customSmartPtr&& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            ref_count = other.ref_count;
            other.ptr = nullptr;
            other.ref_count = nullptr;
        }
        return *this;
    }

    // Destructor
    ~customSmartPtr() {
        cleanup();
    }

    // Deference operator
    T& operator*() const {
        return *ptr;
    };

    // Member selection operator
    T* operator->() const {
        return ptr;
    };

    // Utility function for reference count
    size_t use_count() const {
        return ref_count ? *ref_count : 0;
    }
};