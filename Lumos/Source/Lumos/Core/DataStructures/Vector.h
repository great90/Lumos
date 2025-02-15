#pragma once
#include "Core/OS/Memory.h"
#include <utility> // for std::move

namespace Lumos
{
    template <class T>
    class Vector
    {
    public:
        // Constructors
        Vector(Arena* arena = nullptr);
        Vector(const Vector<T>& other);
        Vector(Vector<T>&& other) noexcept;
        Vector(size_t size, const T& initial = T {}, Arena* arena = nullptr);
        Vector(std::initializer_list<T> values, Arena* arena = nullptr);

        // Destructor
        ~Vector();

        // Copy assignment
        Vector<T>& operator=(const Vector<T>& other);
        // Move assignment
        Vector<T>& operator=(Vector<T>&& other) noexcept;

        // Element access
        T& operator[](size_t index);
        const T& operator[](size_t index) const;

        T& Front();
        const T& Front() const;

        T& Back();
        const T& Back() const;

        // Capacity
        bool Empty() const noexcept;
        size_t Size() const noexcept;
        size_t Capacity() const noexcept;
        void Reserve(size_t capacity);
        void Resize(size_t size, const T& value = T {});

        // Modifiers
        void Clear() noexcept;
        void PushBack(const T& value);
        void PushBack(T&& value);
        template <typename... Args>
        T& EmplaceBack(Args&&... args);
        void PopBack();

        class Iterator
        {
        public:
            Iterator(T* ptr)
                : ptr(ptr)
            {
            }
            Iterator operator++()
            {
                ++ptr;
                return *this;
            }
            bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
            T& operator*() const { return *ptr; }
            Iterator operator+(int offset) const { return Iterator(ptr + offset); }

        private:
            T* ptr;
        };

        class ConstIterator
        {
        public:
            ConstIterator(T* ptr)
                : ptr(ptr)
            {
            }
            ConstIterator operator++()
            {
                ++ptr;
                return *this;
            }
            bool operator!=(const ConstIterator& other) const { return ptr != other.ptr; }
            const T& operator*() const { return *ptr; }
            ConstIterator operator+(int offset) const { return ConstIterator(ptr + offset); }

        private:
            const T* ptr;
        };

        Iterator begin() { return Iterator(m_Data); }
        Iterator end() { return Iterator(m_Data + m_Size); }

        ConstIterator begin() const { return ConstIterator(m_Data); }
        ConstIterator end() const { return ConstIterator(m_Data + m_Size); }

        T* Data() { return m_Data; }

    private:
        T* m_Data         = nullptr;
        size_t m_Size     = 0;
        size_t m_Capacity = 0;
        Arena* m_Arena    = nullptr;

        // Helper function for copying elements
        void CopyElements(const Vector<T>& other);

        // Helper function for deleting allocated memory
        void Destroy() noexcept;
    };

    // Constructor implementations
    template <class T>
    Vector<T>::Vector(Arena* arena)
        : m_Arena(arena)
    {
    }

    template <class T>
    Vector<T>::Vector(const Vector<T>& other)
        : m_Arena(other.m_Arena)
    {
        CopyElements(other);
    }

    template <class T>
    Vector<T>::Vector(Vector<T>&& other) noexcept
        : m_Data(other.m_Data)
        , m_Size(other.m_Size)
        , m_Capacity(other.m_Capacity)
        , m_Arena(other.m_Arena)
    {
        other.m_Data     = nullptr;
        other.m_Size     = 0;
        other.m_Capacity = 0;
    }

    template <class T>
    Vector<T>::Vector(size_t size, const T& initial, Arena* arena)
        : m_Size(0)
        , m_Capacity(0)
        , m_Arena(arena)
    {
        Reserve(size);
        for(size_t i = 0; i < size; ++i)
            m_Data[i] = initial;
    }

    template <class T>
    Vector<T>::Vector(std::initializer_list<T> values, Arena* arena)
        : m_Size(values.size())
        , m_Capacity(0)
        , m_Arena(arena)
    {
        Reserve(m_Size);
        size_t index = 0;
        for(auto& value : values)
            m_Data[index++] = value;
    }

    // Destructor implementation
    template <class T>
    Vector<T>::~Vector()
    {
        Destroy();
    }

    // Copy assignment implementation
    template <class T>
    Vector<T>& Vector<T>::operator=(const Vector<T>& other)
    {
        if(this != &other)
        {
            Destroy();
            CopyElements(other);
            m_Arena = other.m_Arena;
        }
        return *this;
    }

    // Move assignment implementation
    template <class T>
    Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept
    {
        std::swap(m_Data, other.m_Data);
        std::swap(m_Arena, other.m_Arena);
        std::swap(m_Size, other.m_Size);
        std::swap(m_Capacity, other.m_Capacity);

        other.m_Capacity = 0;
        other.m_Data     = nullptr;
        other.m_Arena    = nullptr;

        return *this;
    }

    // Element access implementations
    template <class T>
    T& Vector<T>::operator[](size_t index)
    {
        LUMOS_ASSERT(index < m_Size, "Index must be less than vector's size");
        return m_Data[index];
    }

    template <class T>
    const T& Vector<T>::operator[](size_t index) const
    {
        LUMOS_ASSERT(index < m_Size, "Index must be less than vector's size");
        return m_Data[index];
    }

    template <class T>
    T& Vector<T>::Front()
    {
        return *begin();
    }

    template <class T>
    const T& Vector<T>::Front() const
    {
        return *begin();
    }

    template <class T>
    T& Vector<T>::Back()
    {
        return m_Data[m_Size - 1];
    }

    template <class T>
    const T& Vector<T>::Back() const
    {
        return *(end() - 1);
    }

    // Capacity implementations
    template <class T>
    bool Vector<T>::Empty() const noexcept
    {
        return m_Size == 0;
    }

    template <class T>
    size_t Vector<T>::Size() const noexcept
    {
        return m_Size;
    }

    template <class T>
    size_t Vector<T>::Capacity() const noexcept
    {
        return m_Capacity;
    }

    template <class T>
    void Vector<T>::Reserve(size_t capacity)
    {
        if(capacity <= m_Capacity)
            return;

        T* newData = nullptr;
        if(m_Arena)
            newData = PushArrayNoZero(m_Arena, T, capacity);
        else
            newData = new T[capacity];

        for(size_t i = 0; i < m_Size; ++i)
            newData[i] = std::move(m_Data[i]);

        if(!m_Arena)
            delete[] m_Data;

        m_Data     = newData;
        m_Capacity = capacity;
    }

    template <class T>
    void Vector<T>::Resize(size_t size, const T& value)
    {
        Reserve(size);
        for(size_t i = m_Size; i < size; ++i)
            m_Data[i] = value;
        m_Size = size;
    }

    // Modifiers implementations
    template <class T>
    void Vector<T>::Clear() noexcept
    {
        m_Size = 0;
    }

    template <class T>
    void Vector<T>::PushBack(const T& value)
    {
        if(m_Size == m_Capacity)
            Reserve(m_Capacity == 0 ? 1 : m_Capacity * 2);
        m_Data[m_Size++] = value;
    }

    template <class T>
    void Vector<T>::PushBack(T&& value)
    {
        if(m_Size == m_Capacity)
            Reserve(m_Capacity == 0 ? 1 : m_Capacity * 2);
        m_Data[m_Size++] = std::move(value);
    }

    template <class T>
    template <typename... Args>
    T& Vector<T>::EmplaceBack(Args&&... args)
    {
        if(m_Size == m_Capacity)
            Reserve(m_Capacity == 0 ? 1 : m_Capacity * 2);

        m_Data[m_Size] = T(std::forward<Args>(args)...);
        return m_Data[m_Size++];
    }

    template <class T>
    void Vector<T>::PopBack()
    {
        if(m_Size > 0)
            --m_Size;
    }

    // Helper function implementations
    template <class T>
    void Vector<T>::CopyElements(const Vector<T>& other)
    {
        Reserve(other.m_Capacity);
        for(size_t i = 0; i < other.m_Size; ++i)
            m_Data[i] = other.m_Data[i];
        m_Size = other.m_Size;
    }

    template <class T>
    void Vector<T>::Destroy() noexcept
    {
        if(!m_Arena && m_Data)
            delete[] m_Data;
        m_Data     = nullptr;
        m_Size     = 0;
        m_Capacity = 0;
    }
}
