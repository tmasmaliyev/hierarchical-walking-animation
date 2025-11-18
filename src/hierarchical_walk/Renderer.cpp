#include "hierarchical_walk/Renderer.h"
#include "hierarchical_walk/Constants.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include <cmath>

void drawCylinder(float radius, float height)
{
    GLUquadric *quad = gluNewQuadric();
    gluCylinder(quad, radius, radius, height, 20, 20);
    gluDeleteQuadric(quad);
}

void drawBox(float width, float height, float depth)
{
    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0, 0, 1);
    glVertex3f(-width / 2, 0, depth / 2);
    glVertex3f(width / 2, 0, depth / 2);
    glVertex3f(width / 2, height, depth / 2);
    glVertex3f(-width / 2, height, depth / 2);

    // Back face
    glNormal3f(0, 0, -1);
    glVertex3f(-width / 2, 0, -depth / 2);
    glVertex3f(-width / 2, height, -depth / 2);
    glVertex3f(width / 2, height, -depth / 2);
    glVertex3f(width / 2, 0, -depth / 2);

    // Top face
    glNormal3f(0, 1, 0);
    glVertex3f(-width / 2, height, -depth / 2);
    glVertex3f(-width / 2, height, depth / 2);
    glVertex3f(width / 2, height, depth / 2);
    glVertex3f(width / 2, height, -depth / 2);

    // Bottom face
    glNormal3f(0, -1, 0);
    glVertex3f(-width / 2, 0, -depth / 2);
    glVertex3f(width / 2, 0, -depth / 2);
    glVertex3f(width / 2, 0, depth / 2);
    glVertex3f(-width / 2, 0, depth / 2);

    // Right face
    glNormal3f(1, 0, 0);
    glVertex3f(width / 2, 0, -depth / 2);
    glVertex3f(width / 2, height, -depth / 2);
    glVertex3f(width / 2, height, depth / 2);
    glVertex3f(width / 2, 0, depth / 2);

    // Left face
    glNormal3f(-1, 0, 0);
    glVertex3f(-width / 2, 0, -depth / 2);
    glVertex3f(-width / 2, 0, depth / 2);
    glVertex3f(-width / 2, height, depth / 2);
    glVertex3f(-width / 2, height, -depth / 2);
    glEnd();
}

void drawSphere(float radius)
{
    GLUquadric *quad = gluNewQuadric();
    gluSphere(quad, radius, 20, 20);
    gluDeleteQuadric(quad);
}

void drawLeg(float hipAngle, float kneeAngle)
{
    glPushMatrix();

    // Hip joint rotation
    glRotatef(hipAngle, 1, 0, 0);

    // Upper leg (thigh)
    glColor3f(0.3f, 0.3f, 0.8f);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    drawCylinder(LEG_RADIUS, LEG_LENGTH);
    glPopMatrix();

    // Move to knee position
    glTranslatef(0, -LEG_LENGTH, 0);

    // Knee joint
    glColor3f(0.8f, 0.2f, 0.2f);
    drawSphere(LEG_RADIUS * 1.2);

    // Knee rotation
    glRotatef(kneeAngle, 1, 0, 0);

    // Lower leg (shin)
    glColor3f(0.3f, 0.3f, 0.8f);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    drawCylinder(LEG_RADIUS * 0.9, LEG_LENGTH);
    glPopMatrix();

    // Foot
    glTranslatef(0, -LEG_LENGTH, 0);
    glColor3f(0.6f, 0.4f, 0.2f);
    glPushMatrix();
    glTranslatef(0, -LEG_RADIUS / 2, LEG_RADIUS);
    glScalef(1.5, 0.5, 2.5);
    glBegin(GL_QUADS);
    float s = LEG_RADIUS;
    // Simple cube for foot
    glVertex3f(-s, -s, -s);
    glVertex3f(s, -s, -s);
    glVertex3f(s, s, -s);
    glVertex3f(-s, s, -s);
    glVertex3f(-s, -s, s);
    glVertex3f(-s, s, s);
    glVertex3f(s, s, s);
    glVertex3f(s, -s, s);
    glVertex3f(-s, -s, -s);
    glVertex3f(-s, -s, s);
    glVertex3f(s, -s, s);
    glVertex3f(s, -s, -s);
    glVertex3f(-s, s, -s);
    glVertex3f(s, s, -s);
    glVertex3f(s, s, s);
    glVertex3f(-s, s, s);
    glVertex3f(-s, -s, -s);
    glVertex3f(-s, s, -s);
    glVertex3f(-s, s, s);
    glVertex3f(-s, -s, s);
    glVertex3f(s, -s, -s);
    glVertex3f(s, -s, s);
    glVertex3f(s, s, s);
    glVertex3f(s, s, -s);
    glEnd();
    glPopMatrix();

    glPopMatrix();
}

void drawFigure(const ArticulatedFigure &figure)
{
    glPushMatrix();

    // Position figure
    glTranslatef(figure.position.x, figure.position.y, figure.position.z);

    // Orient figure in forward direction
    float angle = atan2(figure.forward.x, figure.forward.z) * 180.0f / PI;
    glRotatef(angle, 0, 1, 0);

    // Apply body tilt
    glRotatef(figure.bodyTilt, 1, 0, 0);

    // Draw torso
    glColor3f(0.6f, 0.3f, 0.3f);
    drawBox(TORSO_WIDTH, TORSO_HEIGHT, TORSO_DEPTH);

    // Draw left leg
    glPushMatrix();
    glTranslatef(-TORSO_WIDTH * 0.3f, 0, 0);
    drawLeg(figure.leftHipAngle, figure.leftKneeAngle);
    glPopMatrix();

    // Draw right leg
    glPushMatrix();
    glTranslatef(TORSO_WIDTH * 0.3f, 0, 0);
    drawLeg(figure.rightHipAngle, figure.rightKneeAngle);
    glPopMatrix();

    glPopMatrix();
}

void drawSpline(const std::vector<Vec3> &controlPoints, SplineType type)
{
    if (controlPoints.size() < 4)
        return;

    glDisable(GL_LIGHTING);

    // Draw control points
    glPointSize(8.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
    for (const auto &p : controlPoints)
    {
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();

    // Draw spline curve
    glColor3f(0.0f, 1.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (float i = 0; i <= 1.0f; i += 0.01f)
    {
        Vec3 p = (type == CATMULL_ROM) ? evaluateCatmullRom(controlPoints, i) : evaluateBSpline(controlPoints, i);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

void drawGround()
{
    glDisable(GL_LIGHTING);
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_LINES);
    for (int i = -10; i <= 10; i++)
    {
        glVertex3f(i, 0, -10);
        glVertex3f(i, 0, 10);
        glVertex3f(-10, 0, i);
        glVertex3f(10, 0, i);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void initGL(int windowWidth, int windowHeight)
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat light_pos[] = {5.0f, 10.0f, 5.0f, 1.0f};
    GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glShadeModel(GL_SMOOTH);

    // Set up projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)windowWidth / (float)windowHeight, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}