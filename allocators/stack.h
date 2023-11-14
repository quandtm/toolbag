#ifndef TOOLBAG_STACK_H
#define TOOLBAG_STACK_H
#include "../coretypes.h"

namespace allocators
{
class StackAllocator
{
  public:
    typedef void* Marker;

    StackAllocator& operator=(const StackAllocator&) = default;

    bool init(u64 sizeInBytes, void* startPtr);
    void* alloc(u64 sizeInBytes);
    inline void freeToMarker(const Marker m);
    inline void reset();
    Marker getCurrentMarker() const
    {
        return (Marker)sp;
    }

    u64 available() const
    {
        return size - ((ptrdiff_t)sp - (ptrdiff_t)start);
    }
    u64 totalSize() const
    {
        return size;
    }

  private:
    void* start;
    void* sp;
    u64 size;
};
} // namespace allocators
#endif

#ifdef TOOLBAG_STACK_IMPL
namespace allocators
{
bool StackAllocator::init(size_t sizeInBytes, void* startPtr)
{
    if (sizeInBytes == 0 || startPtr == nullptr)
        return false;

    start = startPtr;
    sp = start;
    size = sizeInBytes;

    return true;
}

void* StackAllocator::alloc(size_t sizeInBytes)
{
    if (sizeInBytes == 0 || ((ptrdiff_t)sp + sizeInBytes > ((ptrdiff_t)start + size)))
        return nullptr;
    void* p = sp;
    sp = (void*)((ptrdiff_t)sp + sizeInBytes);
    return p;
}

void StackAllocator::freeToMarker(const Marker m)
{
    if (m == nullptr)
        return;
    sp = m;
}

void StackAllocator::reset()
{
    sp = start;
}
} // namespace allocators
#endif