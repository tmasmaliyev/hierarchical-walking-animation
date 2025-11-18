#ifndef RENDERER_H
#define RENDERER_H

#include "Vec3.h"
#include "ArticulatedFigure.h"
#include "Spline.h"
#include <vector>

// Primitive drawing functions
void drawCylinder(float radius, float height);
void drawBox(float width, float height, float depth);
void drawSphere(float radius);

// Complex drawing functions
void drawLeg(float hipAngle, float kneeAngle);
void drawFigure(const ArticulatedFigure &figure);
void drawSpline(const std::vector<Vec3> &controlPoints, SplineType type);
void drawGround();

// OpenGL initialization
void initGL(int windowWidth, int windowHeight);

#endif // RENDERER_H