#include "MemoryManager.h"
#include <algorithm>
#include <stdexcept>
#include <new>
#include <mutex>

MemoryManager::MemoryManager(size_t totalSize) : totalMemory(totalSize), allocatedMemory(0) {
    // Initialize the memory pool with a single free block
    memoryPool.push_back({nullptr, totalSize, false});
}

void* MemoryManager::allocate(size_t size) {
    std::lock_guard<std::mutex> lock(memoryMutex);

    // Find a free block of sufficient size
    auto blockIt = findFreeBlock(size);

    if (blockIt != memoryPool.end()) {
        // Split the block if it's larger than needed
        if (blockIt->size > size) {
            MemoryBlock newBlock = {
                static_cast<char*>(blockIt->address) + size,
                blockIt->size - size,
                false
            };
            memoryPool.insert(blockIt + 1, newBlock);
            blockIt->size = size;
        }

        // Mark the block as allocated
        blockIt->isAllocated = true;
        allocatedMemory += size;

        return blockIt->address;
    }

    // No suitable block found
    throw std::bad_alloc();
}

void MemoryManager::deallocate(void* ptr) {
    std::lock_guard<std::mutex> lock(memoryMutex);

    // Find the block corresponding to the given pointer
    auto blockIt = std::find_if(memoryPool.begin(), memoryPool.end(),
        [ptr](const MemoryBlock& block) { return block.address == ptr; });

    if (blockIt != memoryPool.end()) {
        // Mark the block as free
        blockIt->isAllocated = false;
        allocatedMemory -= blockIt->size;

        // Merge adjacent free blocks
        mergeAdjacentFreeBlocks();
    } else {
        throw std::runtime_error("Invalid pointer passed to deallocate");
    }
}

std::vector<MemoryBlock>::iterator MemoryManager::findFreeBlock(size_t size) {
    return std::find_if(memoryPool.begin(), memoryPool.end(),
        [size](const MemoryBlock& block) { return !block.isAllocated && block.size >= size; });
}

void MemoryManager::mergeAdjacentFreeBlocks() {
    // Sort memory pool by address
    std::sort(memoryPool.begin(), memoryPool.end(),
        [](const MemoryBlock& a, const MemoryBlock& b) { return a.address < b.address; });

    // Merge adjacent free blocks
    for (auto it = memoryPool.begin(); it != memoryPool.end(); ++it) {
        if (!it->isAllocated) {
            auto nextIt = std::next(it);
            while (nextIt != memoryPool.end() && !nextIt->isAllocated) {
                it->size += nextIt->size;
                it = memoryPool.erase(nextIt);
                nextIt = std::next(it);
            }
        }
    }
}

// Human tasks:
// TODO: Implement memory fragmentation mitigation strategy
// TODO: Add support for aligned memory allocation
// TODO: Implement memory usage statistics and reporting
// TODO: Implement memory leak detection mechanism
// TODO: Add support for deallocation with size hint for better merging
// TODO: Implement periodic defragmentation of memory pool
// TODO: Implement more efficient search algorithm for large memory pools
// TODO: Add support for best-fit or worst-fit allocation strategies
// TODO: Implement more efficient merging algorithm for large memory pools
// TODO: Add support for partial merging to reduce overhead