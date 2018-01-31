#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// OpenGL utility and support function prototypes

void Initialize(GLFWwindow*& window);
void QueryGLVersion();
bool CheckGLErrors();
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
