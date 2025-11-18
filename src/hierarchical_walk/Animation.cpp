#include "hierarchical_walk/Animation.h"
#include "hierarchical_walk/Constants.h"
#include <cmath>

AnimationState::AnimationState()
    : t(0.0f),
      dt(DEFAULT_DT),
      walkCycle(0.0f),
      walkSpeed(DEFAULT_WALK_SPEED),
      animationSpeed(DEFAULT_ANIMATION_SPEED)
{
}

void updateWalkingAnimation(
    ArticulatedFigure &figure,
    AnimationState &state,
    const std::vector<Vec3> &controlPoints,
    SplineType splineType,
    float deltaTime)
{
    if (controlPoints.size() < 4)
        return;

    // Apply animation speed multiplier AND the dt value from file
    deltaTime *= state.animationSpeed * state.dt;

    // Update time parameter
    state.t += deltaTime;
    if (state.t > 1.0f)
        state.t = 0.0f; // Loop animation

    // Get current position and forward direction from spline
    Vec3 newPos = (splineType == CATMULL_ROM) 
        ? evaluateCatmullRom(controlPoints, state.t) 
        : evaluateBSpline(controlPoints, state.t);

    // Calculate velocity for walk speed synchronization
    Vec3 velocity = newPos - figure.position;
    float speed = velocity.length();
    figure.position = newPos;

    // Update forward direction
    figure.forward = getSplineTangent(controlPoints, state.t, splineType);

    // Calculate body tilt based on speed (lean forward when moving faster)
    figure.bodyTilt = speed * 5.0f;
    if (figure.bodyTilt > 15.0f)
        figure.bodyTilt = 15.0f;

    // Update walk cycle based on velocity (prevents moon walking!)
    state.walkCycle += speed * state.walkSpeed * 2.0f;
    if (state.walkCycle > 2 * PI)
        state.walkCycle -= 2 * PI;

    // Calculate leg angles using sinusoidal motion
    // Hip angles: opposite phases for left and right legs
    float hipSwing = 30.0f; // Maximum hip angle in degrees
    figure.leftHipAngle = sin(state.walkCycle) * hipSwing;
    figure.rightHipAngle = sin(state.walkCycle + PI) * hipSwing;

    // Knee angles: bend when leg is forward
    float kneeBend = 20.0f;
    figure.leftKneeAngle = (sin(state.walkCycle) < 0) ? -sin(state.walkCycle) * kneeBend : 0;
    figure.rightKneeAngle = (sin(state.walkCycle + PI) < 0) ? -sin(state.walkCycle + PI) * kneeBend : 0;
}