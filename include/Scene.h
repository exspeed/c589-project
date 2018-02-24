#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#include "Geometry.h"
#include "Camera.h"
#include "Shader.h"

class Scene
{
public:
	Scene(Shader* prog, Shader* progOutline, Camera* cam);
	void AddGeometry(Geometry* g);
	void ClearGeometries();
	void Render() const;
	

private:
	Shader* program;
	Shader* programOutline;
	Camera* camera;


	std::vector<Geometry*> geometries;
	void InitializeVAO(Geometry& g) const;
	void RenderStencil(std::vector<Geometry*> geo) const;
};
