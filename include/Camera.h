#pragma once

#include "PreprocessorSettings.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;

    Camera( glm::vec3 position, glm::vec3 fwd, glm::vec3 head, glm::vec2 windowRes );
    void RotateAround( float deltaX, float deltaY );
    void Zoom( double yoffset );
    void Panning( float deltaX, float deltaY );
    glm::vec3 GetPosition() const;
    glm::vec3 Up() const;
    glm::vec3 LookAtDirection() const;
    float GetFov() const;

    void SetScreenResolution( const glm::vec2 s );
    glm::vec2 GetScreenResolution() const;
    float GetAspectRatio() const;

private:
    const float near = 0.1f;
    const float far = 100.f;

    glm::vec3 pos;
    glm::vec3 target;
    glm::vec3 up;
    glm::vec2 screen;
    float fov;

    void UpdateViewMatrix();
};
