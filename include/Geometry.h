#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "assimp/Exporter.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/mesh.h"

class Geometry
{
public:
	// OpenGL names for array buffer objects, vertex array object
	GLuint  vertexBuffer;
	GLuint  textureBuffer;
	GLuint  colourBuffer;
	GLuint  vertexArray;
	GLenum  renderMode;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> colours;

	Geometry(const Geometry& g);
	Geometry(const std::string filename, GLenum);
	Geometry(std::vector<glm::vec3> v, std::vector<glm::vec3> c, GLenum r);

	void Export(const std::string filename) const;
	void Export(const aiScene* scene) const;
	void Load() const;
	void Destroy() const;

private:
	void InitializeVAO();
};
