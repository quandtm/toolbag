#ifndef TOOLBAG_POOL_H
#define TOOLBAG_POOL_H

namespace allocators
{
    class PoolAllocator
    {
    public:
        PoolAllocator& operator=(const PoolAllocator&) = default;

        bool init(const size_t poolSize, void *startPtr);
        void* alloc(const size_t size);
        void free(void *ptr);

    private:
        struct Header
        {
            size_t length; // Excluding header (in bytes)
            size_t prevLength; // Excluding header (in bytes)
            bool allocated;

            inline Header* prevHeader() const
            {
                return (Header*)((size_t)this - prevLength);
            }

            inline Header* nextHeader() const
            {
                return (Header*)((size_t)this + length);
            }

            inline void* data() const
            {
                return (void*)((size_t)this + sizeof(Header));
            }
        };

        Header *first;
        size_t totalSize;
    };
}
#endif

#ifdef TOOLBAG_POOL_IMPL
namespace allocators
{
    bool PoolAllocator::init(const size_t poolSize, void *startPtr)
    {
        if (poolSize == 0 || startPtr == nullptr || poolSize < sizeof(Header)) return false;
        first = (Header*)startPtr;
        totalSize = poolSize;

        first->length = poolSize - sizeof(Header);
        first->prevLength = 0;
        first->allocated = false;

        return true;
    }

    void* PoolAllocator::alloc(const size_t size)
    {
        // Find the first Header that is not allocated and is >= the requested size
        size_t fullSz = size + sizeof(Header);
        Header *cur = first;
        while ((size_t)cur < ((size_t)first + totalSize))
        {
            if (cur->allocated || cur->length < fullSz)
            {
                cur = cur->nextHeader();
            }
            else
            {
                cur->allocated = true;
                size_t leftover = cur->length - fullSz;
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
        Header *toFree = (Header*)((ptrdiff_t)ptr - sizeof(Header));
        if (!toFree || ((ptrdiff_t)toFree < (ptrdiff_t)first)) return;

        toFree->allocated = false;
        // Check if we need to merge with the next block
        Header *next = toFree->nextHeader();
        Header *nextNext = nullptr;
        if ((size_t)next < ((size_t)first + totalSize))
        {
            if (next->allocated)
            {
                nextNext = next; // Still update prevLength for next block if it is allocated
            }
            else
            {
                toFree->length += next->length;
                if ((size_t)nextNext < ((size_t)first + totalSize))
                    nextNext = next->nextHeader();
            }
        }
        uint64_t blockLen = toFree->length;

        // Check if we need to merge with the previous block
        Header *prev = toFree->prevHeader();
        if (((size_t)prev >= (size_t)first) && !prev->allocated)
        {
            prev->length += toFree->length;
            blockLen = prev->length;
        }

        if (nextNext != nullptr)
            nextNext->prevLength = blockLen;
    }
}
#endif