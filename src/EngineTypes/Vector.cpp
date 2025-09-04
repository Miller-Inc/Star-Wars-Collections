//
// Created by James Miller on 9/3/2025.
//

#include "EngineTypes/Vector.h"
#include <cmath>


MillerInc::mVector4::mVector4()
{
    x = y = z = w = 0.0f;
}

MillerInc::mVector4::mVector4(const float x, const float y, const float z, const float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

MillerInc::mVector4 MillerInc::mVector4::operator+(const mVector4& other) const
{
    return {x+other.x, y+other.y, z+other.z, w+other.w};
}

MillerInc::mVector4 MillerInc::mVector4::operator-(const mVector4& other) const
{
    return {x-other.x, y-other.y, z-other.z, w-other.w};
}

MillerInc::mVector4 MillerInc::mVector4::operator*(float scalar) const
{
    return {x*scalar, y*scalar, z*scalar, w*scalar};
}

MillerInc::mVector4 MillerInc::mVector4::operator/(float scalar) const
{
    return {x/scalar, y/scalar, z/scalar, w/scalar};
}

MillerInc::mVector4& MillerInc::mVector4::operator+=(const mVector4& other)
{
    return *this = *this + other;
}

MillerInc::mVector4& MillerInc::mVector4::operator-=(const mVector4& other)
{
    return *this = *this - other;
}

MillerInc::mVector4& MillerInc::mVector4::operator*=(float scalar)
{
    return *this = *this * scalar;
}

MillerInc::mVector4& MillerInc::mVector4::operator/=(float scalar)
{
    return *this = *this / scalar;
}

float MillerInc::mVector4::Length() const
{
    return sqrt(x*x + y*y + z*z + w*w);
}

MillerInc::mVector4 MillerInc::mVector4::Normalize() const
{
    return *this / Length();
}

float MillerInc::mVector4::Dot(const mVector4& a, const mVector4& b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

MillerInc::mVector4 MillerInc::mVector4::Cross(const mVector4& a, const mVector4& b)
{
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 0.0f}; // w is set to 0 for cross product
}

MillerInc::mVector::mVector()
{
    x = y = z = 0.0f;
}

MillerInc::mVector::mVector(const float x, const float y, const float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

MillerInc::mVector MillerInc::mVector::operator+(const mVector& other) const
{
    return {x+other.x, y+other.y, z+other.z};
}

MillerInc::mVector MillerInc::mVector::operator-(const mVector& other) const
{
    return {x-other.x, y-other.y, z-other.z};
}

MillerInc::mVector MillerInc::mVector::operator*(float scalar) const
{
    return {x*scalar, y*scalar, z*scalar};
}

MillerInc::mVector MillerInc::mVector::operator/(float scalar) const
{
    return {x/scalar, y/scalar, z/scalar};
}

MillerInc::mVector& MillerInc::mVector::operator+=(const mVector& other)
{
    return *this = *this + other;
}

MillerInc::mVector& MillerInc::mVector::operator-=(const mVector& other)
{
    return *this = *this - other;
}

MillerInc::mVector& MillerInc::mVector::operator*=(float scalar)
{
    return *this = *this * scalar;
}

MillerInc::mVector& MillerInc::mVector::operator/=(float scalar)
{
    return *this = *this / scalar;
}

float MillerInc::mVector::Length() const
{
    return sqrt(x*x + y*y + z*z);
}

MillerInc::mVector MillerInc::mVector::Normalize() const
{
    return *this / Length();
}

float MillerInc::mVector::Dot(const mVector& a, const mVector& b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

MillerInc::mVector MillerInc::mVector::Cross(const mVector& a, const mVector& b)
{
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

MillerInc::mVector2::mVector2()
{
    this->x = 0.0f;
    this->y = 0.0f;
}

MillerInc::mVector2::mVector2(const float x, const float y)
{
    this->x = x;
    this->y = y;
}

MillerInc::mVector2 MillerInc::mVector2::operator+(const mVector2& other) const
{
    return {x+other.x, y+other.y};
}

MillerInc::mVector2 MillerInc::mVector2::operator-(const mVector2& other) const
{
    return {x-other.x, y-other.y};
}

MillerInc::mVector2 MillerInc::mVector2::operator*(float scalar) const
{
    return {x*scalar, y*scalar};
}

MillerInc::mVector2 MillerInc::mVector2::operator/(float scalar) const
{
    return {x/scalar, y/scalar};
}

MillerInc::mVector2& MillerInc::mVector2::operator+=(const mVector2& other)
{
    return *this = *this + other;
}

MillerInc::mVector2& MillerInc::mVector2::operator-=(const mVector2& other)
{
    return *this = *this - other;
}

MillerInc::mVector2& MillerInc::mVector2::operator*=(float scalar)
{
    return *this = *this * scalar;
}

MillerInc::mVector2& MillerInc::mVector2::operator/=(float scalar)
{
    return *this = *this / scalar;
}

float MillerInc::mVector2::Length() const
{
    return sqrt(x*x + y*y);
}

MillerInc::mVector2 MillerInc::mVector2::Normalize() const
{
    return *this / Length();
}