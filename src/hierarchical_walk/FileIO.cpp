#include "hierarchical_walk/FileIO.h"
#include "hierarchical_walk/Constants.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool loadControlPoints(
    const char *filename,
    std::vector<Vec3> &controlPoints,
    SplineType &splineType,
    float &dt)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return false;
    }

    controlPoints.clear();
    std::string line;

    // Read spline type
    if (std::getline(file, line))
    {
        if (line.find("CATMULL") != std::string::npos)
        {
            splineType = CATMULL_ROM;
            std::cout << "Using Catmull-Rom spline" << std::endl;
        }
        else if (line.find("BSPLINE") != std::string::npos)
        {
            splineType = BSPLINE;
            std::cout << "Using B-Spline" << std::endl;
        }
    }

    // Read dt value
    if (std::getline(file, line))
    {
        std::istringstream iss(line);
        iss >> dt;
        std::cout << "Time step dt: " << dt << std::endl;
    }

    // Read control points
    float x, y, z;
    while (file >> x >> y >> z)
    {
        controlPoints.push_back(Vec3(x, y, z));
        std::cout << "Control point: (" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    file.close();
    std::cout << "Loaded " << controlPoints.size() << " control points" << std::endl;
    return controlPoints.size() >= 4;
}