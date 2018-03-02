#include "Geometry.h"
#include <iostream>

namespace
{
	const GLuint VERTEX_INDEX = 0;
	const GLuint COLOUR_INDEX = 1;

	const unsigned int VERTEX_COUNT_PER_FACE = 3;
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
	Export("ASD");
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

void Geometry::Export(const std::string filename) const
{
	// Create aiScene, ref: https://github.com/assimp/assimp/issues/1454
	aiScene *scene   = new aiScene();
  scene->mRootNode = new aiNode();

	// Add materials (Currently not supported)
  scene->mMaterials    = new aiMaterial *[1];
  scene->mMaterials[0] = nullptr;
  scene->mNumMaterials = 1;

  scene->mMaterials[0] = new aiMaterial();

	// Create mesh
  scene->mMeshes    = new aiMesh *[1];
  scene->mMeshes[0] = nullptr;
  scene->mNumMeshes = 1;

  scene->mMeshes[0] = new aiMesh();
  scene->mMeshes[0]->mMaterialIndex = 0;

  scene->mRootNode->mMeshes    = new unsigned int[1];
  scene->mRootNode->mMeshes[0] = 0;
  scene->mRootNode->mNumMeshes = 1;

  auto pMesh = scene->mMeshes[0];

  pMesh->mVertices    = new aiVector3D[vertices.size()];
  pMesh->mNumVertices = vertices.size();

	// Fill mesh with verticecs
	for(int i = 0; i < vertices.size(); ++i)
	{
		pMesh->mVertices[i] = aiVector3D(vertices[i].x, vertices[i].y, vertices[i].z);
	}

	// Fill face data
	pMesh->mNumFaces = vertices.size() / VERTEX_COUNT_PER_FACE;
	pMesh->mFaces    = new aiFace[pMesh->mNumFaces];

	for(int i = 0; i < pMesh->mNumFaces; ++i) {
		aiFace& face     = pMesh->mFaces[i];
		face.mIndices    = new unsigned int[3];
		face.mNumIndices = VERTEX_COUNT_PER_FACE;

		for(int j = 0; j < VERTEX_COUNT_PER_FACE; ++j)
		{
			int idx = (i * VERTEX_COUNT_PER_FACE) + j;
			face.mIndices[j] = idx;
		}
	}

	// Export
	Assimp::Exporter exporter;
	const char* formatId = "obj";
	const char* exportDir = "exports/";
	auto ret = exporter.Export(scene, formatId, exportDir + filename + "." + formatId);
	if(ret != AI_SUCCESS) std::cout << "FAIL" << std::endl;
	else std::cout << "SUCCESS" << std::endl;
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

// Deallocate geometry-related objects
void Geometry::Destroy() const 
{
	// Destroy our vertex array object and associated buffers
	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colourBuffer);
}
