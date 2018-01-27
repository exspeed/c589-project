#pragma once

#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --------------------------------------------------------------------------
// Functions to set up OpenGL shader programs for rendering
// --------------------------------------------------------------------------

std::string LoadSource(const std::string& filename);
GLuint CompileShader(GLenum shaderType, const std::string& source);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
