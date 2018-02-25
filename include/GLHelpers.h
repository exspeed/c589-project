#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

// OpenGL utility and support function prototypes

void Initialize(GLFWwindow*& window);
void QueryGLVersion();
bool CheckGLErrors();
void ErrorCallback(int error, const char* description);

void PrintMat4(glm::mat4 m);
void PrintVec3(glm::vec3 v);
