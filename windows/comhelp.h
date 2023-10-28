#pragma once

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
        Destroy();
    }
    explicit ComPtr(T* const Target) noexcept : Ptr(Target)
    {
    }
    explicit ComPtr(const ComPtr<T>& Other) noexcept = delete;
    explicit ComPtr(ComPtr<T>&& Other) noexcept
    {
        Ptr = Other.Ptr;
        Other.Ptr = nullptr;
    }

    ComPtr<T>& operator=(const ComPtr<T>&) noexcept = delete;
    ComPtr<T>& operator=(ComPtr<T>&& Other) noexcept
    {
        if (this == &Other)
        {
            return *this;
        }

        Destroy();
        Ptr = Other.Ptr;
        Other.Ptr = nullptr;

        return *this;
    }

    bool operator==(const ComPtr<T>& rhs) const noexcept
    {
        return Ptr == rhs.Ptr;
    }

    bool operator==(const T* rhs) const noexcept
    {
        return Ptr == rhs;
    }

    bool IsValid() const
    {
        return Ptr != nullptr;
    }

    T* operator->() const
    {
        return *Ptr;
    }

    T* Get() const
    {
        return Ptr;
    }

  private:
    void Destroy()
    {
        SafeRelease(Ptr);
    }

    T* Ptr = nullptr;
};
