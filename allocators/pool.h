#ifndef TOOLBAG_POOL_H
#define TOOLBAG_POOL_H
#include "../coretypes.h"

namespace allocators
{
class PoolAllocator
{
  public:
    PoolAllocator& operator=(const PoolAllocator&) = default;

    bool init(const u64 poolSize, void* startPtr);
    void* alloc(const u64 size);
    void free(void* ptr);

  private:
    struct Header
    {
        u64 length;     // Excluding header (in bytes)
        u64 prevLength; // Excluding header (in bytes)
        bool allocated;

        inline Header* prevHeader() const
        {
            return (Header*)((u64)this - prevLength);
        }

        inline Header* nextHeader() const
        {
            return (Header*)((u64)this + length);
        }

        inline void* data() const
        {
            return (void*)((u64)this + sizeof(Header));
        }
    };

    Header* first;
    u64 totalSize;
};
} // namespace allocators
#endif

#ifdef TOOLBAG_POOL_IMPL
namespace allocators
{
bool PoolAllocator::init(const u64 poolSize, void* startPtr)
{
    if (poolSize == 0 || startPtr == nullptr || poolSize < sizeof(Header))
        return false;
    first = (Header*)startPtr;
    totalSize = poolSize;

    first->length = poolSize - sizeof(Header);
    first->prevLength = 0;
    first->allocated = false;

    return true;
}

void* PoolAllocator::alloc(const u64 size)
{
    // Find the first Header that is not allocated and is >= the requested size
    u64 fullSz = size + sizeof(Header);
    Header* cur = first;
    while ((u64)cur < ((u64)first + totalSize))
    {
        if (cur->allocated || cur->length < fullSz)
        {
            cur = cur->nextHeader();
        }
        else
        {
            cur->allocated = true;
            u64 leftover = cur->length - fullSz;
            cur->length = fullSz;
            // Only split if the leftover space can fit another header + some more data
            if (leftover > sizeof(Header))
            {
                Header* next = cur->nextHeader();
                next->allocated = false;
                next->length = leftover;
                next->prevLength = cur->length;
            }
            return cur->data();
        }
    }
    return nullptr;
}

void PoolAllocator::free(void* ptr)
{
    Header* toFree = (Header*)((ptrdiff_t)ptr - sizeof(Header));
    if (!toFree || ((ptrdiff_t)toFree < (ptrdiff_t)first))
        return;

    toFree->allocated = false;
    // Check if we need to merge with the next block
    Header* next = toFree->nextHeader();
    Header* nextNext = nullptr;
    if ((u64)next < ((u64)first + totalSize))
    {
        if (next->allocated)
        {
            nextNext = next; // Still update prevLength for next block if it is allocated
        }
        else
        {
            toFree->length += next->length;
            if ((u64)nextNext < ((u64)first + totalSize))
                nextNext = next->nextHeader();
        }
    }
    u64 blockLen = toFree->length;

    // Check if we need to merge with the previous block
    Header* prev = toFree->prevHeader();
    if (((u64)prev >= (u64)first) && !prev->allocated)
    {
        prev->length += toFree->length;
        blockLen = prev->length;
    }

    if (nextNext != nullptr)
        nextNext->prevLength = blockLen;
}
} // namespace allocators
#endif