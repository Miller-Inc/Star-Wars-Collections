//
// Created by James Miller on 9/3/2025.
//

#pragma once

namespace MillerInc
{
    typedef struct mVector2
    {
        float x;
        float y;

        mVector2();
        mVector2(float x, float y);
        mVector2 operator+(const mVector2& other) const;
        mVector2 operator-(const mVector2& other) const;
        mVector2 operator*(float scalar) const;
        mVector2 operator/(float scalar) const;
        mVector2& operator+=(const mVector2& other);
        mVector2& operator-=(const mVector2& other);
        mVector2& operator*=(float scalar);
        mVector2& operator/=(float scalar);
        [[nodiscard]] float Length() const;
        [[nodiscard]] mVector2 Normalize() const;
    } MVector2;

    typedef struct mVector
    {
        float x;
        float y;
        float z;

        mVector();
        mVector(float x, float y, float z);
        mVector operator+(const mVector& other) const;
        mVector operator-(const mVector& other) const;
        mVector operator*(float scalar) const;
        mVector operator/(float scalar) const;
        mVector& operator+=(const mVector& other);
        mVector& operator-=(const mVector& other);
        mVector& operator*=(float scalar);
        mVector& operator/=(float scalar);
        [[nodiscard]] float Length() const;
        [[nodiscard]] mVector Normalize() const;
        static float Dot(const mVector& a, const mVector& b);
        static mVector Cross(const mVector& a, const mVector& b);

    } MVector;

    typedef struct mVector4
    {
        float x;
        float y;
        float z;
        float w;

        mVector4();
        mVector4(float x, float y, float z, float w);
        mVector4 operator+(const mVector4& other) const;
        mVector4 operator-(const mVector4& other) const;
        mVector4 operator*(float scalar) const;
        mVector4 operator/(float scalar) const;
        mVector4& operator+=(const mVector4& other);
        mVector4& operator-=(const mVector4& other);
        mVector4& operator*=(float scalar);
        mVector4& operator/=(float scalar);
        [[nodiscard]] float Length() const;
        [[nodiscard]] mVector4 Normalize() const;
        static float Dot(const mVector4& a, const mVector4& b);
        static mVector4 Cross(const mVector4& a, const mVector4& b);
    } MVector4;

    typedef mVector2 MSize;
};