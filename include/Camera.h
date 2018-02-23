#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera
{
public:
// attributes
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

//functions
void rotateAroundFocus(float deltaX, float deltaY);
void rotateAround(glm::vec3& vec, glm::vec3 const &axis, float radian);
Camera();

private:
// attributes
float m_focusDist;
glm::vec3 m_pos;
glm::vec3 m_up;
glm::vec3 m_forward;

//functions
void updateViewMatrix();

};