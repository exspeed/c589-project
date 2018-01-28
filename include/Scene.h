#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Geometry.h"

class Scene
{
public:
	Scene(std::string vertex_shader, std::string fragment_shader);
	void AddGeometry(Geometry& g);
	void ClearGeometries();
	void Render() const;

private:
	GLuint program;
	std::vector<Geometry> geometries;
	void InitializeVAO(Geometry& g) const;
	void DestroyGeometry(Geometry& g) const;
	void LoadGeometry(Geometry& g) const;
};
