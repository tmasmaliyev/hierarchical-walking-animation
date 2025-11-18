#include "hierarchical_walk/Vec3.h"
#include <cmath>

Vec3::Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Vec3 Vec3::operator+(const Vec3 &v) const
{
    return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3 &v) const
{
    return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator*(float s) const
{
    return Vec3(x * s, y * s, z * s);
}

float Vec3::length() const
{
    return sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalize() const
{
    float len = length();
    return len > 0 ? Vec3(x / len, y / len, z / len) : Vec3(0, 0, 0);
}