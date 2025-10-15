//
// Created by James Miller on 9/9/2025.
//

#pragma once
#include <cstddef>
#include <stdexcept>
#include <cstring>
#include <cstdlib> // For malloc, realloc, free
#include <utility> // For std::move
#include "EngineTypes/Logger.h"
#include <vector>
#include <algorithm> // For std::copy

namespace MillerInc
{
    template<typename T>
    class MArray final
    {
    public:
        MArray() = default;
        ~MArray();
        MArray(const MArray& other) // Copy constructor
        {
            length = other.length;
            capacity = other.capacity;
            if (capacity > 0)
            {
                data = (T*)malloc(sizeof(T) * capacity);
                std::copy(other.data, other.data + length, data);
            } else
            {
                data = nullptr;
            }
        }
        MArray(MArray&& other) noexcept // Move constructor
        {
            data = other.data;
            length = other.length;
            capacity = other.capacity;
            other.data = nullptr;
            other.length = 0;
            other.capacity = 0;
        }
        MArray& operator=(const MArray& other) // Copy assignment
        {
            if (this != &other)
            {
                Clear();
                length = other.length;
                capacity = other.capacity;
                if (capacity > 0)
                {
                    data = (T*)malloc(sizeof(T) * capacity);
                    std::copy(other.data, other.data + length, data);
                } else
                {
                    data = nullptr;
                }
            }
            return *this;
        }
        MArray& operator=(MArray&& other) noexcept // Move assignment
        {
            if (this != &other)
            {
                Clear();
                data = other.data;
                length = other.length;
                capacity = other.capacity;
                other.data = nullptr;
                other.length = 0;
                other.capacity = 0;
            }
            return *this;
        }

        MArray(std::vector<T> vec) // Construct from std::vector
        {
            length = vec.size();
            capacity = length;
            if (capacity > 0)
            {
                data = (T*)malloc(sizeof(T) * capacity);
                std::copy(vec.begin(), vec.end(), data);
            } else
            {
                data = nullptr;
            }
        }

        [[nodiscard]] T& operator[](size_t index);
        MArray& operator+(const MArray& other) const; // Concatenate two arrays
        void operator+=(const MArray& other); // Append another array to this one
        void operator+=(const T& element);
        [[nodiscard]] T& Get(size_t index);
        [[nodiscard]] size_t Len() const;
        [[nodiscard]] bool IsEmpty() const;
        void RemoveAt(size_t index);
        void Add(const T& element);
        void Clear();
        void Resize(size_t newCapacity);
        void AddRange(const MArray& other);
        T* begin() { return data; }
        T* end() { return data + length; }
        const T* begin() const { return data; }
        const T* end() const { return data + length; }

        size_t Find(const T& element) const;

    private:
        T* data = nullptr;
        size_t length = 0;
        size_t capacity = 0;

    };

    template <typename T>
    MArray<T>::~MArray()
    {
        Clear(); // Free allocated memory
    }

    template <typename T>
    T& MArray<T>::operator[](size_t index)
    {
        if (index >= capacity)
        {
            M_LOGGER(Logger::LogCore, Logger::Error, "Index out of range in MArray::operator[]");
            throw std::out_of_range("Index out of range");
        }

        return data[index];
    }

    template <typename T>
    MArray<T>& MArray<T>::operator+(const MArray& other) const
    {
        auto* newArray = new MArray<T>();
        newArray->Resize(length + other.length);
        for (size_t i = 0; i < length; i++)
        {
            newArray->Add(data[i]);
        }
        for (size_t i = 0; i < other.length; i++)
        {
            newArray->Add(other.data[i]);
        }
        return *newArray;
    }

    template <typename T>
    void MArray<T>::operator+=(const MArray& other)
    {
        if (length + other.length > capacity)
        {
            Resize(length + other.length);
        }

        if (other.length > 0)
        {
            memcpy(data + length, other.data, sizeof(T) * other.length);
            length += other.length;
        }
    }

    template <typename T>
    void MArray<T>::operator+=(const T& element)
    {
        Add(element);
    }

    template <typename T>
    T& MArray<T>::Get(size_t index)
    {
        return operator[](index);
    }

    template <typename T>
    size_t MArray<T>::Len() const
    {
        return length;
    }

    template <typename T>
    bool MArray<T>::IsEmpty() const
    {
        return length == 0;
    }

    template <typename T>
    void MArray<T>::RemoveAt(size_t index)
    {
        if (index >= length)
        {
            M_LOGGER(Logger::LogCore, Logger::Error, "Index out of range");
            throw std::out_of_range("Index out of range");
        }

        if (index < length - 1)
        {
            memmove(&data[index], &data[index + 1], sizeof(T) * (length - index - 1));
        }

        length--;
    }

    template <typename T>
    void MArray<T>::Add(const T& element)
    {
        if (length >= capacity)
        {
            Resize(capacity == 0 ? 1 : capacity * 2);
        }
        data[length++] = element;
    }

    template <typename T>
    void MArray<T>::Clear()
    {
        free(data);
        data = nullptr;
        length = 0;
        capacity = 0;
    }

    template <typename T>
    void MArray<T>::Resize(size_t newCapacity)
    {
        if (data != nullptr)
        {
            data = (T*)realloc(data, sizeof(T) * newCapacity);
            capacity = newCapacity;
            if (length >= capacity)
            {
                length = capacity;
            }
        } else {
            data = (T*)malloc(sizeof(T) * newCapacity);
            capacity = newCapacity;
            length = 0;
        }

    }

    template <typename T>
    void MArray<T>::AddRange(const MArray& other)
    {
        *this += other;
    }

    template <typename T>
    size_t MArray<T>::Find(const T& element) const
    {
        if (length == 0)
            return -1;
        for (size_t i = 0; i < length; i++)
        {
            if (element == data[i])
                return i;
        }
        return -1;
    }
}
