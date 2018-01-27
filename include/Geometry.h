#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

struct Geometry
{
	// OpenGL names for array buffer objects, vertex array object
	GLuint  vertexBuffer;
	GLuint  textureBuffer;
	GLuint  colourBuffer;
	GLuint  vertexArray;

	std::vector<glm::vec2> vertices;
	std::vector<glm::vec3> colours;

	Geometry(const Geometry& g);
	Geometry(std::vector<glm::vec2> v, std::vector<glm::vec3> c);
};