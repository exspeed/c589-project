#include <glad/glad.h>

#include "Scene.h"
#include "Camera.h"

Scene::Scene(Shader* prog, Shader* progOutline, Camera* cam):
program(prog)
, programOutline(progOutline)
, camera(cam)
{

}

void Scene::AddGeometry(Geometry* g)
{
	geometries.push_back(g);
}

void Scene::ClearGeometries()
{
	for(auto geometry : geometries)
	{
		delete geometry;
	}
	geometries.clear();
}


// Rendering function that draws our scene to the frame buffer
void Scene::Render() const
{
	
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);
	// Draw non stencil objects here
	// TODO ...
	
	RenderStencil(geometries);
	
	glEnable(GL_DEPTH_TEST);
	
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);
}


void Scene::RenderStencil(std::vector<Geometry*> geo) const
{
	program->use();
	
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	
	glm::mat4 model = glm::mat4(1.0); // placeholder
	glm::mat4 MVP = camera->ProjectionMatrix* camera->ViewMatrix * model;
	
	program->setMat4("MVP", MVP);
	
	for(auto geometry : geometries)
	{
		// bind our shader program and the vertex array object containing our
		// scene geometry, then tell OpenGL to draw our geometry
		glBindVertexArray(geometry->vertexArray);
		glDrawArrays(geometry->renderMode, 0, geometry->vertices.size());
	}
	
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	
	programOutline->use();
	
	model = glm::scale (glm::vec3(1.1f, 1.1f, 1.1f));
	MVP =camera->ProjectionMatrix* camera->ViewMatrix * model;
	programOutline->setMat4("MVP", MVP);
	
	for(auto geometry : geo)
	{
		glBindVertexArray(geometry->vertexArray);
		glDrawArrays(geometry->renderMode, 0, geometry->vertices.size());
	}
	glBindVertexArray(0);
	glStencilMask(0xFF);

}









