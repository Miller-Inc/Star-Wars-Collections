//
// Created by James Miller on 9/9/2025.
//

#pragma once
#include <cstddef>
#include <stdexcept>
#include <cstring>
#include <utility> // For std::move
#include "EngineTypes/Logger.h"

namespace MillerInc
{
    template<typename T>
    class MArray final
    {
    public:
        MArray() = default;
        ~MArray();

        [[nodiscard]] T& operator[](size_t index);
        MArray& operator+(const MArray& other) const; // Concatenate two arrays
        void operator+=(const MArray& other); // Append another array to this one
        void operator+=(const T& element);
        [[nodiscard]] T& Get(size_t index);
        [[nodiscard]] size_t Len() const;
        void RemoveAt(size_t index);
        void Add(const T& element);
        void Clear();
        void Resize(size_t newCapacity);
        void AddRange(const MArray& other);
        T* begin() { return data; }
        T* end() { return data + length; }
        const T* begin() const { return data; }
        const T* end() const { return data + length; }

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
    void MArray<T>::RemoveAt(size_t index)
    {
        if (index >= length)
        {
            M_LOGGER(Logger::LogCore, Logger::Error, "Index out of range");
            throw std::out_of_range("Index out of range");
        }

        // | 0 | 1 | 2 | 3 | 4 | (len = 5)
        // RemoveAt(2)
        // | 0 | 1 | 3 | 4 |

        if (index < length - 1)
        {
            memcpy(data[index], data[index + 1], (sizeof(T) * (length - index - 1)));
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
        delete[] data;
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

}
