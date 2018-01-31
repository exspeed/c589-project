#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Geometry
{
public:
	// OpenGL names for array buffer objects, vertex array object
	GLuint  vertexBuffer;
	GLuint  textureBuffer;
	GLuint  colourBuffer;
	GLuint  vertexArray;

	std::vector<glm::vec2> vertices;
	std::vector<glm::vec3> colours;

	Geometry(const Geometry& g);
	Geometry(std::vector<glm::vec2> v, std::vector<glm::vec3> c);

	void Load() const;
	void Destroy() const;

private:
	void InitializeVAO();
};
