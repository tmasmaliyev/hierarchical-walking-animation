#include "hierarchical_walk/ArticulatedFigure.h"

ArticulatedFigure::ArticulatedFigure()
    : position(0, 0, 0),
      forward(0, 0, 1),
      bodyTilt(0),
      leftHipAngle(0),
      rightHipAngle(0),
      leftKneeAngle(0),
      rightKneeAngle(0)
{
}