#ifndef ANIMATION_H
#define ANIMATION_H

#include "ArticulatedFigure.h"
#include "Vec3.h"
#include "Spline.h"
#include <vector>

struct AnimationState
{
    float t;              // Current time parameter for spline
    float dt;             // Time step
    float walkCycle;      // Phase of walking cycle [0, 2*PI]
    float walkSpeed;      // Speed multiplier
    float animationSpeed; // Overall animation speed multiplier
    
    AnimationState();
};

// Update the walking animation
void updateWalkingAnimation(
    ArticulatedFigure &figure,
    AnimationState &state,
    const std::vector<Vec3> &controlPoints,
    SplineType splineType,
    float deltaTime
);

#endif // ANIMATION_H