#ifndef VEC3_H
#define VEC3_H

#include <cmath>

struct Vec3
{
    float x, y, z;
    
    Vec3(float _x = 0, float _y = 0, float _z = 0);
    
    Vec3 operator+(const Vec3 &v) const;
    Vec3 operator-(const Vec3 &v) const;
    Vec3 operator*(float s) const;
    
    float length() const;
    Vec3 normalize() const;
};

#endif // VEC3_H