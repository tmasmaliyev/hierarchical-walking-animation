#include "hierarchical_walk/Spline.h"

Vec3 evaluateCatmullRom(const std::vector<Vec3> &points, float t)
{
    if (points.size() < 4)
        return Vec3(0, 0, 0);

    int numSegments = points.size() - 3;
    float segmentT = t * numSegments;
    int segment = (int)segmentT;

    if (segment >= numSegments)
    {
        segment = numSegments - 1;
        segmentT = 1.0f;
    }
    else
    {
        segmentT = segmentT - segment;
    }

    // Get the 4 control points for this segment
    Vec3 p0 = points[segment];
    Vec3 p1 = points[segment + 1];
    Vec3 p2 = points[segment + 2];
    Vec3 p3 = points[segment + 3];

    float t2 = segmentT * segmentT;
    float t3 = t2 * segmentT;

    // Catmull-Rom matrix coefficients
    Vec3 result;
    result.x = 0.5f * ((2 * p1.x) + (-p0.x + p2.x) * segmentT +
                       (2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * t2 +
                       (-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * t3);
    result.y = 0.5f * ((2 * p1.y) + (-p0.y + p2.y) * segmentT +
                       (2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * t2 +
                       (-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * t3);
    result.z = 0.5f * ((2 * p1.z) + (-p0.z + p2.z) * segmentT +
                       (2 * p0.z - 5 * p1.z + 4 * p2.z - p3.z) * t2 +
                       (-p0.z + 3 * p1.z - 3 * p2.z + p3.z) * t3);

    return result;
}

Vec3 evaluateBSpline(const std::vector<Vec3> &points, float t)
{
    if (points.size() < 4)
        return Vec3(0, 0, 0);

    int numSegments = points.size() - 3;
    float segmentT = t * numSegments;
    int segment = (int)segmentT;

    if (segment >= numSegments)
    {
        segment = numSegments - 1;
        segmentT = 1.0f;
    }
    else
    {
        segmentT = segmentT - segment;
    }

    Vec3 p0 = points[segment];
    Vec3 p1 = points[segment + 1];
    Vec3 p2 = points[segment + 2];
    Vec3 p3 = points[segment + 3];

    float t2 = segmentT * segmentT;
    float t3 = t2 * segmentT;

    // B-Spline basis functions
    Vec3 result;
    result.x = (1.0f / 6.0f) * ((-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * t3 +
                                (3 * p0.x - 6 * p1.x + 3 * p2.x) * t2 +
                                (-3 * p0.x + 3 * p2.x) * segmentT +
                                (p0.x + 4 * p1.x + p2.x));
    result.y = (1.0f / 6.0f) * ((-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * t3 +
                                (3 * p0.y - 6 * p1.y + 3 * p2.y) * t2 +
                                (-3 * p0.y + 3 * p2.y) * segmentT +
                                (p0.y + 4 * p1.y + p2.y));
    result.z = (1.0f / 6.0f) * ((-p0.z + 3 * p1.z - 3 * p2.z + p3.z) * t3 +
                                (3 * p0.z - 6 * p1.z + 3 * p2.z) * t2 +
                                (-3 * p0.z + 3 * p2.z) * segmentT +
                                (p0.z + 4 * p1.z + p2.z));

    return result;
}

Vec3 getSplineTangent(const std::vector<Vec3> &points, float t, SplineType type)
{
    float epsilon = 0.001f;
    Vec3 p1 = (type == CATMULL_ROM) ? evaluateCatmullRom(points, t) : evaluateBSpline(points, t);
    Vec3 p2 = (type == CATMULL_ROM) ? evaluateCatmullRom(points, t + epsilon) : evaluateBSpline(points, t + epsilon);
    return (p2 - p1).normalize();
}