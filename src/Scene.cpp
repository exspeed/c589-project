#include <glad/glad.h>

#include "Scene.h"
#include "ShaderTool.h"
#include "Control.h"

Scene::Scene(std::string vertex_shader, std::string fragment_shader)
{
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

void Scene::AddGeometry(Geometry& g)
{
	InitializeVAO(g);
	LoadGeometry(g);
	geometries.push_back(std::move(g));
}

void Scene::ClearGeometries()
{
	for(auto geometry : geometries)
	{
		DestroyGeometry(geometry);
	}
	geometries.clear();
}

// Rendering function that draws our scene to the frame buffer
void Scene::Render() const
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);

    
    calculateMVP();
    glm::mat4 Projection = getProjectionMatrix();
    glm::mat4 View = getViewMatrix();
    glm::mat4 Model = glm::mat4();

    glm::mat4  MVP = Projection * View * Model;
    
    glUniformMatrix4fv(glGetUniformLocation(program,"MVP"), 1, GL_FALSE, &MVP[0][0]);
    
	for(auto geometry : geometries)
	{
		// bind our shader program and the vertex array object containing our
		// scene geometry, then tell OpenGL to draw our geometry
		glBindVertexArray(geometry.vertexArray);
		glDrawArrays(GL_TRIANGLES, 0, geometry.vertices.size());

		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
		glUseProgram(0);
	}
}

void Scene::InitializeVAO(Geometry& g) const
{
	const GLuint VERTEX_INDEX = 0;
	const GLuint COLOUR_INDEX = 1;

	//Generate Vertex Buffer Objects
	// create an array buffer object for storing our vertices
	glGenBuffers(1, &g.vertexBuffer);

	// create another one for storing our colours
	glGenBuffers(1, &g.colourBuffer);

	//Set up Vertex Array Object
	// create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &g.vertexArray);
	glBindVertexArray(g.vertexArray);

	// associate the position array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, g.vertexBuffer);
	glVertexAttribPointer(
		VERTEX_INDEX,		//Attribute index 
		2, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(glm::vec2),		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(VERTEX_INDEX);

	// associate the colour array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, g.colourBuffer);
	glVertexAttribPointer(
		COLOUR_INDEX,		//Attribute index 
		3, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		sizeof(glm::vec3), 		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(COLOUR_INDEX);

	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Deallocate geometry-related objects
void Scene::DestroyGeometry(Geometry& g) const 
{
	// unbind and destroy our vertex array object and associated buffers
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &g.vertexArray);
	glDeleteBuffers(1, &g.vertexBuffer);
	glDeleteBuffers(1, &g.colourBuffer);
}

void Scene::LoadGeometry(Geometry& g) const
{
	// create an array buffer object for storing our vertices
	glBindBuffer(GL_ARRAY_BUFFER, g.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * g.vertices.size(), &g.vertices.front(), GL_STATIC_DRAW);

	// create another one for storing our colours
	glBindBuffer(GL_ARRAY_BUFFER, g.colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * g.colours.size(), &g.colours.front(), GL_STATIC_DRAW);

	//Unbind buffer to reset to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
