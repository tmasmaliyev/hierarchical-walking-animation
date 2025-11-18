#ifndef FILEIO_H
#define FILEIO_H

#include "Vec3.h"
#include "Spline.h"
#include <vector>

// Load control points from file
bool loadControlPoints(
    const char *filename,
    std::vector<Vec3> &controlPoints,
    SplineType &splineType,
    float &dt
);

#endif // FILEIO_H