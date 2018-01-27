// ==========================================================================
// Barebones OpenGL Core Profile Boilerplate
//    using the GLFW windowing system (http://www.glfw.org)
//
// Loosely based on
//  - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
//  - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)
//
// Author:  Sonny Chan, University of Calgary
// Co-Authors:
//			Jeremy Hart, University of Calgary
//			John Hall, University of Calgary
// Date:    December 2015
// ==========================================================================

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Geometry.h"
#include "Scene.h"
#include "ShaderTool.h"
// --------------------------------------------------------------------------
// OpenGL utility and support function prototypes

void QueryGLVersion();
bool CheckGLErrors();
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// PROGRAM ENTRY POINT
int main(int argc, char *argv[])
{
	// initialize the GLFW windowing system
	if (!glfwInit()) {
		std::cerr << "ERROR: GLFW failed to initialize, TERMINATING" << std::endl;
		return -1;
	}
	glfwSetErrorCallback(ErrorCallback);

	// Attempt to create a window with an OpenGL 4.1 core profile context
	GLFWwindow *window = 0;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 512, height = 512;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		std::cerr << "Program failed to create GLFW window, TERMINATING" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set keyboard callback function and make our context current (active)
	glfwSetKeyCallback(window, KeyCallback);
	glfwMakeContextCurrent(window);

	//Intialize GLAD
	if (!gladLoadGL())
	{
		std::cerr << "GLAD init failed" << std::endl;
		return -1;
	}

	// Query and print out information about our OpenGL environment
	QueryGLVersion();

	// Create Geometry
	std::vector<glm::vec2> vertices =
	{
		glm::vec2( -.6f, -.4f ),
		glm::vec2( .0f,  .6f ),
		glm::vec2( .6f, -.4f )
	};

	std::vector<glm::vec3> colours =
	{
		glm::vec3( 1.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ),
		glm::vec3( 0.0f, 0.0f, 1.0f )
	};

	Geometry geometry(vertices, colours);

	// Create Scene
	Scene scene("shaders/vertex.glsl", "shaders/fragment.glsl");
	scene.AddGeometry(geometry);

	CheckGLErrors();

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// call function to draw our scene
		scene.Render();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// Clean up allocated resources before exit
	scene.ClearGeometries();
	glUseProgram(0);
	glfwDestroyWindow(window);
	glfwTerminate();

	std::cout << "Goodbye!" << std::endl;
	return 0;
}

// OpenGL utility functions

// Reports OpenGL version and renderer information
void QueryGLVersion()
{
	std::string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	std::string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	std::string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	std::cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << std::endl;
}

// Reports OpenGL errors
bool CheckGLErrors()
{
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		std::cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM" << std::endl; break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE" << std::endl; break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION" << std::endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY" << std::endl; break;
		default:
			std::cout << "[unknown error code]" << std::endl;
		}
		error = true;
	}
	return error;
}

// GLFW CALLBACK FUNCTIONS

// Reports GLFW errors
void ErrorCallback(int error, const char* description)
{
	std::cerr << "GLFW ERROR " << error << ":" << std::endl;
	std::cerr << description << std::endl;
}

// Handles keyboard input events
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
