#pragma once
#include <guiddef.h>

template <typename ComType> void SafeRelease(ComType*& Ptr)
{
    if (Ptr != nullptr)
    {
        Ptr->Release();
        Ptr = nullptr;
    }
}

template <typename T> class ComPtr
{
  public:
    ComPtr() noexcept = default;
    ~ComPtr() noexcept
    {
        Reset();
    }
    ComPtr(T* const Target) noexcept : Ptr(Target)
    {
    }
    explicit ComPtr(const ComPtr<T>& Other) noexcept = delete;
    explicit ComPtr(ComPtr<T>&& Other) noexcept
    {
        Ptr = Other.Ptr;
        Other.Ptr = nullptr;
    }

    ComPtr<T>& operator=(const ComPtr<T>&) noexcept = delete;
    ComPtr<T>& operator=(ComPtr<T>& Other) noexcept
    {
        if (Ptr == Other.Ptr)
        {
            return *this;
        }

        Reset();
        Ptr = Other.Ptr;
        Other.Ptr = nullptr;

        return *this;
    }
    ComPtr<T>& operator=(ComPtr<T>&& Other) noexcept
    {
        if (Ptr == Other.Ptr)
        {
            return *this;
        }

        Reset();
        Ptr = Other.Ptr;
        Other.Ptr = nullptr;

        return *this;
    }
    ComPtr<T>& operator=(T*&& Other) noexcept
    {
        if (Ptr == Other)
        {
            return *this;
        }
        Reset();
        Ptr = Other;
        return *this;
    }

    template <typename OtherType> bool As(ComPtr<OtherType>& Other) const
    {
        if (IsValid())
        {
            OtherType* tempPtr;
            if (Ptr->QueryInterface(&tempPtr) >= 0)
            {
                Other = tempPtr;
                return Other.IsValid();
            }
        }
        return false;
    }

    template <typename OtherType> bool From(const ComPtr<OtherType>& Other)
    {
        return Other.As(*this);
    }

    [[nodiscard]] bool operator==(const ComPtr<T>& rhs) const noexcept
    {
        return Ptr == rhs.Ptr;
    }

    [[nodiscard]] bool operator==(const T* rhs) const noexcept
    {
        return Ptr == rhs;
    }

    [[nodiscard]] bool IsValid() const
    {
        return Ptr != nullptr;
    }

    T* operator->() const
    {
        return Ptr;
    }

    [[nodiscard]] T* Get() const
    {
        return Ptr;
    }

    [[nodiscard]] T** GetAddress()
    {
        return &Ptr;
    }

    [[nodiscard]] void** GetVoidAddress()
    {
        return reinterpret_cast<void**>(&Ptr);
    }

    [[nodiscard]] const IID& GetUUID() const
    {
        return __uuidof(T);
    }

    void Reset()
    {
        SafeRelease(Ptr);
    }

  protected:
    T* Ptr = nullptr;
};

#define IIDOF(comptr) (comptr).GetUUID(), (comptr).GetVoidAddress()
