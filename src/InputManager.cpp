#include "InputManager.h"

InputManager::InputManager( GLFWwindow* w, Camera* cam, Scene* s )
    : window( w )
    , camera( cam )
    , scene( s ) {
}

void InputManager::CheckInput() {
    KeyInput();
    KeyRotate();
    MouseInput();
}

void InputManager::MouseInput() {
    double xpos, ypos;

    if ( shiftKey && glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS ) {
        glfwGetCursorPos( window, &xpos, &ypos );
        float deltaX = ( xpos - cursorX ) * mouseSensitivity;
        float deltaY = ( ypos - cursorY ) * mouseSensitivity;

        if ( !( deltaX || deltaY ) ) {
            return;
        }

        camera->Panning( deltaX, deltaY );
    } else if ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS ) {
        // Get mouse position
        glfwGetCursorPos( window, &xpos, &ypos );
        float deltaX = ( xpos - cursorX ) * mouseSensitivity;
        float deltaY = ( ypos - cursorY ) * mouseSensitivity;

        if ( !( deltaX || deltaY ) ) {
            return;
        }

        camera->RotateAround( deltaX, deltaY );
    }

    glfwGetCursorPos( window, &xpos, &ypos );
    cursorX = xpos;
    cursorY = ypos;
}

void InputManager::KeyInput() {
    if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
        glfwSetWindowShouldClose( window, GL_TRUE );
    } else if ( glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS ) {
        camera->Zoom( true );
    } else if ( glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS ) {
        camera->Zoom( false );
    } else if ( glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS ) {
        shiftKey = true;
    } else {
        shiftKey = false;
    }

    // Z key
    {
        static int oldState = GLFW_RELEASE;
        int newState = glfwGetKey( window, GLFW_KEY_Z );
        if ( newState == GLFW_RELEASE && oldState == GLFW_PRESS ) {
            if ( wireframe ){
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            wireframe = !wireframe;
        }
        oldState = newState;
    }

    for ( int i = 0; i < scene->GetGeometriesSize() && i < 9; i++ ) {
        static int oldState = GLFW_RELEASE;
        int newState = glfwGetKey( window, GLFW_KEY_0 + i + 1 );

        if ( newState == GLFW_RELEASE && oldState == GLFW_PRESS ) {
            scene->ToggleSelectedGeometry( i );
        }

        oldState = newState;
    }
}

void InputManager::KeyRotate() {
    for ( int i = 0; i < scene->GetGeometriesSize(); i++ ) { 
        Geometry* geometry = scene->getGeometry( i );
        if ( geometry->IsSelectedGeometry() ) {
            if ( glfwGetKey( window, GLFW_KEY_Q ) == GLFW_PRESS ) {
                geometry->ModelMatrix = glm::rotate(geometry->ModelMatrix, 0.05f, glm::vec3(0.0f, 1.0f, 0.0f));
            } else if (glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS ) {
                geometry->ModelMatrix = glm::rotate(geometry->ModelMatrix, -0.05f, glm::vec3(0.0f, 1.0f, 0.0f));
            }
        }
    }
}

void InputManager::ScrollWheel( double xoffset, double yoffset ) {
    if ( yoffset == 1) {
        camera->Zoom( true );
    } else if ( yoffset == -1 ) {
        camera->Zoom( false );
    }
}



