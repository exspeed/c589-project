#pragma once

#include "PreprocessorSettings.h"

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/mesh.h"

#include "cork.h"

class Geometry {
public:
    // OpenGL names for array buffer objects, vertex array object
    GLuint  vertexBuffer;
    GLuint  textureBuffer;
    GLuint  colourBuffer;
    GLuint  normalBuffer;

    GLuint  vertexArray;
    GLenum  renderMode;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colours;
    std::vector<glm::vec3> normals;

    glm::mat4 ModelMatrix;

    Geometry( const Geometry& g );
    Geometry( const std::string filename, GLenum );
    Geometry( std::vector<glm::vec3> v, std::vector<glm::vec3> c, std::vector<glm::vec3> n, GLenum r );
    Geometry( const CorkTriMesh& trimesh, GLenum r );

    void ToggleSelectedGeometry();
    bool IsSelectedGeometry();

    void Load() const;
    void Destroy() const;

    void GetCorkTriMesh( CorkTriMesh& out );

private:
    bool selected = 0;
    void InitializeVAO();
};
