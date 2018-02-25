#pragma once

#define GLFW_INCLUDE_NONE

#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// --------------------------------------------------------------------------
// Functions to set up OpenGL shader programs for rendering
// Reference: https://learnopengl.com/
// --------------------------------------------------------------------------


class Shader
{
public:
	
	Shader(std::string vertex_shader, std::string fragment_shader);
	
	void use();
	void setInt(const std::string &name, int value) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	
private:
	GLuint programID;	
	
	std::string LoadSource(const std::string& filename);
	GLuint CompileShader(GLenum shaderType, const std::string& source);
	GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

	

};
