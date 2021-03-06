#pragma once

#include "PreprocessorSettings.h"

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Shader.h"
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
    GLuint  faceBuffer;

    GLuint  vertexArray;
    GLenum  renderMode;

    Shader* program;
    Shader* programOutline;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colours;
    std::vector<glm::vec3> normals;
    std::vector<GLuint> faces;

    glm::mat4 ModelMatrix;

    Geometry( const Geometry& g );
    Geometry( const std::string filename, GLenum r, Shader* geo = nullptr, Shader* stencil = nullptr );
    Geometry( std::vector<glm::vec3> v, std::vector<glm::vec3> c, std::vector<glm::vec3> n, GLenum r, Shader* geo = nullptr, Shader* stencil = nullptr );
    Geometry( const CorkTriMesh& trimesh, GLenum r, Shader* geo = nullptr, Shader* stencil = nullptr );

    void ToggleSelectedGeometry();
    bool IsSelectedGeometry();

    void Load() const;
    void Destroy() const;

    void Scale( const glm::vec3 scale );
    void Translate( const glm::vec3 translate );
    void Rotate( const glm::vec3 rotate, const float angle );

    void SmoothLine();
    std::vector<glm::vec3> ArcLengthParameterize( std::vector<glm::vec3> vertices );
    std::vector<glm::vec3> Subdivide( std::vector<glm::vec3> vertices );
    std::vector<glm::vec3> ChaikinReverseSubdivision( std::vector<glm::vec3> F );
    std::vector<glm::vec3> ChaikinSubdivision( std::vector<glm::vec3> C );


    static Geometry* Crack( Geometry* inp, Geometry* crack );

private:
    bool selected = false;
    void InitializeVAO();

    void GetCorkTriMesh( CorkTriMesh& out );

    void RemoveDegeneracies();

    // Returns Union of two geometries (OR)
    Geometry* operator+( Geometry& g );
    // Returns Difference of two geometries (NOT)
    Geometry* operator-( Geometry& g );
    // Returns Intersection of two geometries (AND)
    Geometry* operator*( Geometry& g );
    // Returns Symmetric Difference of two geometries (XOR)
    Geometry* operator/( Geometry& g );
};
