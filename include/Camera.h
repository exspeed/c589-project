#pragma once

#include "PreprocessorSettings.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;

    Camera( glm::vec3 position, glm::vec3 fwd, glm::vec3 head );
    void RotateAround( float deltaX, float deltaY );
    void Zoom( bool in );
    void Panning( float deltaX, float deltaY );

private:
    glm::vec3 pos;
    glm::vec3 forward;
    glm::vec3 up;

    void UpdateViewMatrix();
};
