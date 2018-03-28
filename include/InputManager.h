#pragma once

#include "Camera.h"
#include "Scene.h"

class InputManager {
public:
    GLFWwindow* window;
    Camera* camera;
    Scene* scene;

    InputManager( GLFWwindow* window, Camera* cam, Scene* s );
    void ScrollWheel( double xoffset, double yoffset );
    void CheckInput();

    void KeyInput( const int key, const int action );

private:
    float cursorX = 0.0f;
    float cursorY = 0.0f;
    bool shiftKey = false;
    bool wireframe = false;
    bool sketching = false;

    void MouseInput();
};
