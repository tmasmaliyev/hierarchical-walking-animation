#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <iostream>

#include "hierarchical_walk/Constants.h"
#include "hierarchical_walk/Vec3.h"
#include "hierarchical_walk/ArticulatedFigure.h"
#include "hierarchical_walk/Spline.h"
#include "hierarchical_walk/Renderer.h"
#include "hierarchical_walk/Animation.h"
#include "hierarchical_walk/FileIO.h"

// ============================================================================
// GLOBAL STATE
// ============================================================================

GLFWwindow *window = nullptr;
int windowWidth = DEFAULT_WINDOW_WIDTH;
int windowHeight = DEFAULT_WINDOW_HEIGHT;

ArticulatedFigure figure;
AnimationState animState;
std::vector<Vec3> controlPoints;
SplineType splineType = CATMULL_ROM;

double lastFrameTime = 0.0;

// Camera parameters
float cameraDistance = DEFAULT_CAMERA_DISTANCE;
float cameraAngleX = DEFAULT_CAMERA_ANGLE_X;
float cameraAngleY = DEFAULT_CAMERA_ANGLE_Y;
double lastMouseX = 0.0, lastMouseY = 0.0;
bool mousePressed = false;
bool firstMouse = true;

// ============================================================================
// GLFW CALLBACKS
// ============================================================================

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_EQUAL: // '+' key
        case GLFW_KEY_KP_ADD:
            animState.animationSpeed += 0.01f;
            std::cout << "Animation speed: " << animState.animationSpeed << std::endl;
            break;
        case GLFW_KEY_MINUS:
        case GLFW_KEY_KP_SUBTRACT:
            animState.animationSpeed -= 0.01f;
            if (animState.animationSpeed < 0.01f)
                animState.animationSpeed = 0.01f;
            std::cout << "Animation speed: " << animState.animationSpeed << std::endl;
            break;
        case GLFW_KEY_W:
            animState.walkSpeed += 0.1f;
            std::cout << "Walk speed (leg movement): " << animState.walkSpeed << std::endl;
            break;
        case GLFW_KEY_S:
            animState.walkSpeed -= 0.1f;
            if (animState.walkSpeed < 0.1f)
                animState.walkSpeed = 0.1f;
            std::cout << "Walk speed (leg movement): " << animState.walkSpeed << std::endl;
            break;
        case GLFW_KEY_R:
            animState.t = 0.0f;
            animState.walkCycle = 0.0f;
            std::cout << "Animation reset" << std::endl;
            break;
        }
    }
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            mousePressed = true;
            firstMouse = true;
        }
        else if (action == GLFW_RELEASE)
        {
            mousePressed = false;
        }
    }
}

void cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (mousePressed)
    {
        if (firstMouse)
        {
            lastMouseX = xpos;
            lastMouseY = ypos;
            firstMouse = false;
        }

        double xOffset = xpos - lastMouseX;
        double yOffset = ypos - lastMouseY;

        lastMouseX = xpos;
        lastMouseY = ypos;

        cameraAngleY += xOffset * 0.5f;
        cameraAngleX += yOffset * 0.5f;

        if (cameraAngleX > 89.0f)
            cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f)
            cameraAngleX = -89.0f;
    }
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    cameraDistance -= yoffset * 0.5f;
    if (cameraDistance < 2.0f)
        cameraDistance = 2.0f;
    if (cameraDistance > 20.0f)
        cameraDistance = 20.0f;
}

// ============================================================================
// RENDERING LOOP
// ============================================================================

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up camera
    float camX = cameraDistance * sin(cameraAngleY * PI / 180.0f) * cos(cameraAngleX * PI / 180.0f);
    float camY = cameraDistance * sin(cameraAngleX * PI / 180.0f);
    float camZ = cameraDistance * cos(cameraAngleY * PI / 180.0f) * cos(cameraAngleX * PI / 180.0f);

    gluLookAt(figure.position.x + camX, figure.position.y + camY + 2, figure.position.z + camZ,
              figure.position.x, figure.position.y + 1, figure.position.z,
              0, 1, 0);

    // Draw scene
    drawGround();
    drawSpline(controlPoints, splineType);
    drawFigure(figure);
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main(int argc, char **argv)
{
    std::cout << "=== CS6555 Hierarchical Walking Animation (GLFW) ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  Mouse drag: Rotate camera" << std::endl;
    std::cout << "  Mouse wheel: Zoom in/out" << std::endl;
    std::cout << "  +/- keys: Adjust overall animation speed" << std::endl;
    std::cout << "  W/S keys: Adjust leg movement speed" << std::endl;
    std::cout << "  R key: Reset animation" << std::endl;
    std::cout << "  ESC: Exit" << std::endl;
    std::cout << std::endl;

    // Load control points
    const char *filename = (argc > 1) ? argv[1] : "control_points.txt";
    if (!loadControlPoints(filename, controlPoints, splineType, animState.dt))
    {
        std::cerr << "Failed to load control points. Using default path." << std::endl;

        // Create default circular path
        for (int i = 0; i < 8; i++)
        {
            float angle = i * 2 * PI / 8;
            controlPoints.push_back(Vec3(3 * cos(angle), 0, 3 * sin(angle)));
        }
        // Duplicate first few points for proper spline evaluation
        for (int i = 0; i < 3; i++)
        {
            controlPoints.push_back(controlPoints[i]);
        }
    }

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create window
    window = glfwCreateWindow(windowWidth, windowHeight,
                              "Hierarchical Walking Animation",
                              nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    // Set callbacks
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Initialize OpenGL
    initGL(windowWidth, windowHeight);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    lastFrameTime = glfwGetTime();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        // Update animation
        updateWalkingAnimation(figure, animState, controlPoints, splineType, deltaTime);

        // Render
        render();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}