#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Geometry.h"
#include "Camera.h"

class Scene
{
public:
	Scene(std::string vertex_shader, std::string fragment_shader,Camera* cam);
	void AddGeometry(Geometry* g);
	void ClearGeometries();
	void Render() const;
	

private:
	Camera* camera;
	GLuint program;
	std::vector<Geometry*> geometries;
	void InitializeVAO(Geometry& g) const;
};
