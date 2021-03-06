#pragma once

#include "../renderer/Camera.h"
#include "SettingsToolbox.h"

#include <GLFW/glfw3.h>

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

class Window {
public:
    Window(Camera* camera);
    ~Window();

    bool shouldClose();
    void swapBuffers();
    void pollEvents();
    double getTimeSinceLastFrame();
    const char* getProcessAddress();

    static int width;
    static int height;

    static void cursorPositionCallback(GLFWwindow*, double, double);
    static void mouseButtonCallback(GLFWwindow*, int, int , int);
    static void windowSizeChangedCallback(GLFWwindow*, int, int);
    static void keyCallback(GLFWwindow*, int, int, int, int); 
private:
    GLFWwindow* window;

    double lastFrameTime;

    static SettingsToolbox* settingsToolbox;
    static bool hasMouseMoved;
    static bool exitKeyWasPressed;
    static bool cameraIsBeingMoved;
    static float mouseXPosition;
    static float mouseYPosition;
    static Camera* camera;


};
