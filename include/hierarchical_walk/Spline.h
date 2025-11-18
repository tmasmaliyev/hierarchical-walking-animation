#ifndef SPLINE_H
#define SPLINE_H

#include "Vec3.h"
#include <vector>

enum SplineType
{
    CATMULL_ROM,
    BSPLINE
};

// Catmull-Rom spline evaluation
Vec3 evaluateCatmullRom(const std::vector<Vec3> &points, float t);

// B-Spline evaluation
Vec3 evaluateBSpline(const std::vector<Vec3> &points, float t);

// Get tangent vector for forward direction
Vec3 getSplineTangent(const std::vector<Vec3> &points, float t, SplineType type);

#endif // SPLINE_H