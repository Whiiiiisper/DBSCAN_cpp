#pragma once
#include <cstdint>
#include <type_traits>

template <typename T> class LinearAllocator {
    static_assert(std::is_trivially_destructible<T>::value, "T must be trivially destructible");

public:
    LinearAllocator(size_t size) : _mem_size(size * sizeof(T)), _init_addr(new char[size * sizeof(T)]), _next_addr(_init_addr) {}

    ~LinearAllocator() { delete[] _init_addr; }

    LinearAllocator(const LinearAllocator &) = delete;
    LinearAllocator &operator=(const LinearAllocator &) = delete;

    T *allocate()
    {
        if (static_cast<size_t>(_next_addr - _init_addr) >= _mem_size) {
            throw std::bad_alloc();
        }

        T *result = reinterpret_cast<T *>(_next_addr);
        _next_addr += sizeof(T);
        return new (result) T;
    }

    void deallocate(T *ptr)
    {
        // Do nothing because T is trivially destructible
    }

    void reset() { _next_addr = _init_addr; }

    bool isDeallocSupported() { return false; }

private:
    size_t _mem_size;
    char *_init_addr;
    char *_next_addr;
};