#include <glad/glad.h>

#include "Geometry.h"

namespace
{
	const GLuint VERTEX_INDEX = 0;
	const GLuint COLOUR_INDEX = 1;
}

Geometry::Geometry(const Geometry& g)
	: vertexBuffer(g.vertexBuffer)
	, colourBuffer(g.colourBuffer)
	, vertexArray(g.vertexArray)
	, vertices(g.vertices)
	, colours(g.colours)
	, renderMode(g.renderMode)
{
	InitializeVAO();
	Load();
}

Geometry::Geometry(const std::string filename, GLenum r)
	: vertexBuffer(0)
	, colourBuffer(0)
	, vertexArray(0)
	, vertices({})
	, colours({})
	, renderMode(r)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename, NULL);
	if(!scene) 
	{
		printf("Unable to load mesh: %s\n", importer.GetErrorString());
	}

	for(int i = 0; i < scene->mNumMeshes; i++)
	{
		auto mesh = scene->mMeshes[i];
		for(int j = 0; j < mesh->mNumVertices; j++)
		{
			aiVector3t<float> vec = mesh->mVertices[j];
			vertices.push_back(glm::vec3(vec.x, vec.y, vec.z));
			colours.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		}
	}

	InitializeVAO();
	Load();
}

Geometry::Geometry(std::vector<glm::vec3> v, std::vector<glm::vec3> c, GLenum r)
	: vertexBuffer(0)
	, colourBuffer(0)
	, vertexArray(0)
	, vertices(std::move(v))
	, colours(std::move(c))
	, renderMode(r)
{
	InitializeVAO();
	Load();
}

void Geometry::InitializeVAO()
{
	// Generate Vertex Buffer Objects
	// Create an array buffer object for storing our vertices
	glGenBuffers(1, &vertexBuffer);

	// Create another one for storing our colours
	glGenBuffers(1, &colourBuffer);

	// Set up Vertex Array Object
	// Create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// Associate the position array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		VERTEX_INDEX,		//Attribute index 
		3, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		0,		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(VERTEX_INDEX);

	// Associate the colour array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glVertexAttribPointer(
		COLOUR_INDEX,		//Attribute index 
		3, 					//# of components
		GL_FLOAT, 			//Type of component
		GL_FALSE, 			//Should be normalized?
		0, 		//Stride - can use 0 if tightly packed
		0);					//Offset to first element
	glEnableVertexAttribArray(COLOUR_INDEX);

	// Unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Geometry::Load() const
{
	// create an array buffer object for storing our vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);

	// create another one for storing our colours
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colours.size(), &colours.front(), GL_STATIC_DRAW);

	//Unbind buffer to reset to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Geometry::SetSelectedGeometry()
{
	selected = selected ? 0 : 1;
}

bool Geometry::IsSelectedGeometry()
{
	return selected;
}


// Deallocate geometry-related objects
void Geometry::Destroy() const 
{
	// Destroy our vertex array object and associated buffers
	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colourBuffer);
}
