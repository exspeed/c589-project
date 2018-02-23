#include <glad/glad.h>

#include "Scene.h"
#include "ShaderTool.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


Scene::Scene(std::string vertex_shader, std::string fragment_shader)
{
	Camera cam;
	camera = cam;

	// load shader source from files
	std::string vertexSource = LoadSource(vertex_shader);
	std::string fragmentSource = LoadSource(fragment_shader);
	if (vertexSource.empty() || fragmentSource.empty())
	{
		std::cerr << "Program could not initialize shaders, TERMINATING" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	// compile shader source into shader objects
	GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	// link shader program
	program = LinkProgram(vertex, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
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
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);

	for(auto geometry : geometries)
	{
		// bind our shader program and the vertex array object containing our
		// scene geometry, then tell OpenGL to draw our geometry

		// calculate translation, scale, and rotation matrices
		glm::mat4 identity = glm::mat4(1.0f);
		glm::mat4 scale = glm::scale(identity, geometry->scale);
		glm::mat4 rotate = glm::rotate(identity, geometry->rotateDegree, geometry->rotateAxis);
		glm::mat4 translate = glm::translate(identity, geometry->translate);

		// create model matrix
		glm::mat4 model = translate * rotate * scale;		// Applies scale -> rotate -> translate
		glm::mat4 MVP = camera.ProjectionMatrix* camera.ViewMatrix * model;
		glUniformMatrix4fv(glGetUniformLocation(program,"MVP"), 1, GL_FALSE, &MVP[0][0]);
		glBindVertexArray(geometry->vertexArray);
		glDrawArrays(geometry->renderMode, 0, geometry->vertices.size());
	}

	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);
}
