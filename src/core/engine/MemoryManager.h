#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <cstddef>
#include <vector>
#include <mutex>

// Forward declaration of MemoryBlock
class MemoryBlock;

class MemoryManager {
public:
    // Constructor: Initializes the MemoryManager with a specified total memory size
    explicit MemoryManager(size_t totalSize);

    // Allocates a block of memory of the specified size
    void* allocate(size_t size);

    // Deallocates a previously allocated memory block
    void deallocate(void* ptr);

    // Returns the amount of currently allocated memory
    size_t getUsedMemory() const;

    // Returns the total amount of managed memory
    size_t getTotalMemory() const;

private:
    std::vector<MemoryBlock> memoryPool;
    size_t totalMemory;
    size_t allocatedMemory;
    mutable std::mutex memoryMutex;
};

class MemoryBlock {
public:
    // Constructor: Initializes a MemoryBlock with the given address and size
    MemoryBlock(void* addr, size_t blockSize, bool free = true);

    void* address;
    size_t size;
    bool isFree;
};

// Human tasks:
// TODO: Add comprehensive documentation for each method, including usage examples and best practices
// TODO: Implement thread-safe versions of getUsedMemory and getTotalMemory methods
// TODO: Consider adding a method for memory defragmentation
// TODO: Add support for memory allocation strategies (e.g., best-fit, worst-fit, first-fit)
// TODO: Implement a method to get detailed memory usage statistics
// TODO: Consider adding support for memory allocation with alignment requirements
// TODO: Implement exception handling for out-of-memory situations

#endif // MEMORY_MANAGER_H