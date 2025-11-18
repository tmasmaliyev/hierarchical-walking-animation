#ifndef ARTICULATED_FIGURE_H
#define ARTICULATED_FIGURE_H

#include "Vec3.h"

struct ArticulatedFigure
{
    Vec3 position;        // Position of torso base
    Vec3 forward;         // Forward direction
    float bodyTilt;       // Lean angle
    float leftHipAngle;   // Left leg hip rotation
    float rightHipAngle;  // Right leg hip rotation
    float leftKneeAngle;  // Left leg knee rotation
    float rightKneeAngle; // Right leg knee rotation

    ArticulatedFigure();
};

#endif // ARTICULATED_FIGURE_H