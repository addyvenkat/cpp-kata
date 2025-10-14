#pragma once

#include <cstddef>
#include <memory>
#include <utility>

#ifdef USE_UNIQUE_PTR
// Using unique_ptr
class CMoveOnlyBuffer
{
private:
    std::unique_ptr<uint32_t> _dataPtr;
public:
    // Constructor
    explicit CMoveOnlyBuffer(uint32_t val)
    {
        this->_dataPtr = std::make_unique<uint32_t>(val);
    }

    // Move constructor
    CMoveOnlyBuffer(CMoveOnlyBuffer&& other) noexcept
    {
        this->_dataPtr = std::exchange(other._dataPtr, nullptr);
    }

    // Move assignment - default
    CMoveOnlyBuffer& operator=(CMoveOnlyBuffer&& other) noexcept
    {
        this->_dataPtr = std::exchange(other._dataPtr, nullptr);
        return *this;
    }
    
    // Destructor
    ~CMoveOnlyBuffer() = default;
    
    CMoveOnlyBuffer(const CMoveOnlyBuffer&) = delete; // Copy constructor - delete
    CMoveOnlyBuffer& operator=(const CMoveOnlyBuffer&) = delete; // Copy assignment - delete 

    uint32_t* getDataPtr() noexcept { return this->_dataPtr.get(); }
    uint32_t getVal() noexcept {return (_dataPtr) ? *_dataPtr : 0;}
};

#else
// Using new-delete
class CMoveOnlyBuffer
{
private:
    uint32_t * _dataPtr{};
public:
    // Constructor
    explicit CMoveOnlyBuffer(uint32_t val)
    {
        this->_dataPtr = new uint32_t(val);
    }

    // Move constructor
    CMoveOnlyBuffer(CMoveOnlyBuffer&& other) noexcept
    {
        this->_dataPtr = other._dataPtr;
        other._dataPtr = nullptr;
    }

    // Move assignment - default
    CMoveOnlyBuffer& operator=(CMoveOnlyBuffer&& other) noexcept
    {
        if(this != &other)
        {
            delete this->_dataPtr;
            this->_dataPtr = other._dataPtr;
            other._dataPtr = nullptr;
        }
        return *this;
    }
    
    // Destructor
    ~CMoveOnlyBuffer() noexcept
    {
        delete this->_dataPtr;
    }
    
    CMoveOnlyBuffer(const CMoveOnlyBuffer&) = delete; // Copy constructor - delete
    CMoveOnlyBuffer& operator=(const CMoveOnlyBuffer&) = delete; // Copy assignment - delete 

    uint32_t* getDataPtr() noexcept { return this->_dataPtr; }
    uint32_t getVal() noexcept {return (_dataPtr) ? *_dataPtr : 0;}
};
#endif // USE_UNIQUE_PTR

static_assert(!std::is_copy_constructible_v<CMoveOnlyBuffer>);
static_assert(!std::is_copy_assignable_v<CMoveOnlyBuffer>);
static_assert(std::is_nothrow_move_constructible_v<CMoveOnlyBuffer>);
static_assert(std::is_nothrow_move_assignable_v<CMoveOnlyBuffer>);
