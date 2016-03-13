#pragma once
#include <stdint.h>

namespace allocators
{
    class PoolAllocator
    {
    public:
        PoolAllocator& operator=(const PoolAllocator&) = default;

        bool init(const uint64_t poolSize, void *startPtr);
        void* alloc(const uint64_t size);
        void free(void *ptr);
        
    private:
        struct Header
        {
            uint32_t length; // Excluding header (in bytes)
            uint32_t prevLength; // Excluding header (in bytes)
            bool allocated;

            inline Header* prevHeader() const
            {
                return (Header*)((uint64_t)this - prevLength);
            }

            inline Header* nextHeader() const
            {
                return (Header*)((uint64_t)this + length);
            }

            inline void* data() const
            {
                return (void*)((uint64_t)this + sizeof(Header));
            }
        };

        Header *first;
        uint64_t totalSize;
    };

    bool PoolAllocator::init(const uint64_t poolSize, void *startPtr)
    {
        if (poolSize == 0 || startPtr == nullptr || poolSize < sizeof(Header)) return false;
        first = (Header*)startPtr;
        totalSize = poolSize;

        first->length = poolSize - sizeof(Header);
        first->prevLength = 0;
        first->allocated = false;

        return true;
    }

    void* PoolAllocator::alloc(const uint64_t size)
    {
        // Find the first Header that is not allocated and is >= the requested size
        uint64_t fullSz = size + sizeof(Header);
        Header *cur = first;
        while ((uint64_t)cur < ((uint64_t)first + totalSize))
        {
            if (cur->allocated || cur->length < fullSz)
            {
                cur = cur->nextHeader();
            }
            else
            {
                cur->allocated = true;
                uint64_t leftover = cur->length - fullSz;
                cur->length = fullSz;
                // Only split if the leftover space can fit another header + some more data
                if (leftover > sizeof(Header))
                {
                    Header *next = cur->nextHeader();
                    next->allocated = false;
                    next->length = leftover;
                    next->prevLength = cur->length;
                }
                return cur->data();
            }
        }
        return nullptr;
    }

    void PoolAllocator::free(void *ptr)
    {
        Header *toFree = (Header*)((uint64_t)ptr - sizeof(Header));
        if (!toFree || ((uint64_t)toFree < (uint64_t)first)) return;

        toFree->allocated = false;
        // Check if we need to merge with the next block
        Header *next = toFree->nextHeader();
        Header *nextNext = nullptr;
        if ((uint64_t)next < ((uint64_t)first + totalSize))
        {
            if (next->allocated)
            {
                nextNext = next; // Still update prevLength for next block if it is allocated
            }
            else
            {
                toFree->length += next->length;
                if ((uint64_t)nextNext < ((uint64_t)first + totalSize))
                    nextNext = next->nextHeader();
            }
        }
        uint64_t blockLen = toFree->length;

        // Check if we need to merge with the previous block
        Header *prev = toFree->prevHeader();
        if ((uint64_t)prev >= (uint64_t)first && !prev->allocated)
        {
            prev->length += toFree->length;
            blockLen = prev->length;
        }

        if (nextNext != nullptr)
            nextNext->prevLength = blockLen;
    }
}