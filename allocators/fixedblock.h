#pragma once
#include <stdint.h>

namespace allocators
{
    class FixedBlockAllocator
    {
    public:
        FixedBlockAllocator& operator=(const FixedBlockAllocator&) = default;

        bool init(uint64_t blockSize, uint64_t blockCount, void *startPtr);
        void* alloc();
        void free(void *ptr);
        inline bool canAlloc() { return next != nullptr; }

    private:
        void *next;
    };

    bool FixedBlockAllocator::init(uint64_t blockSize, uint64_t blockCount, void *startPtr)
    {
        if (blockSize == 0 || blockCount == 0 || startPtr == nullptr || blockSize < sizeof(void*)) return false;
        next = startPtr;

        // Setup initial state of linked list
        for (uint64_t block = 0; block < blockCount - 1; ++block)
        {
            void **p = (void**)((ptrdiff_t)startPtr + (block * blockSize));
            *p = (void*)((ptrdiff_t)startPtr + ((block + 1) + blockSize));
        }
        *(void**)((ptrdiff_t)startPtr + ((blockCount - 1) * blockSize)) = nullptr;

        return true;
    }

    inline void* FixedBlockAllocator::alloc()
    {
        if (!canAlloc()) return nullptr; // No space available
        void *n = next;
        next = *((void**)next);
        return n;
    }

    inline void FixedBlockAllocator::free(void *ptr)
    {
        if (ptr == nullptr) return;
        *((void**)ptr) = next;
        next = ptr;
    }
}