#include <glad/glad.h>

#include "assimp/postprocess.h"
#include "Geometry.h"

namespace {
    const GLuint VERTEX_INDEX = 0;
    const GLuint COLOUR_INDEX = 1;
    const GLuint NORMAL_INDEX = 2;
}

Geometry::Geometry( const Geometry& g )
    : vertexBuffer( g.vertexBuffer )
    , textureBuffer( g.textureBuffer )
    , colourBuffer( g.colourBuffer )
    , normalBuffer( g.normalBuffer )
    , vertexArray( g.vertexArray )
    , renderMode( g.renderMode )
    , vertices( g.vertices )
    , colours( g.colours )
    , normals( g.normals )
    , ModelMatrix( g.ModelMatrix ) {
    InitializeVAO();
    Load();
}

Geometry::Geometry( const std::string filename, GLenum r )
    : vertexBuffer( 0 )
    , textureBuffer( 0 )
    , colourBuffer( 0 )
    , normalBuffer( 0 )
    , vertexArray( 0 )
    , renderMode( r )
    , vertices( {} )
, colours( {} )
, normals( {} )
, ModelMatrix( glm::mat4( 1.0f ) ) {
    Assimp::Importer importer;
    auto import_flags = aiProcess_FindDegenerates | aiProcess_FindInvalidData | aiProcess_FixInfacingNormals | aiProcess_ImproveCacheLocality | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;
    const aiScene* scene = importer.ReadFile( filename, import_flags );

    if ( !scene ) {
        printf( "Unable to load mesh: %s\n", importer.GetErrorString() );
    }

    for ( int i = 0; i < scene->mNumMeshes; i++ ) {
        auto mesh = scene->mMeshes[i];

        for ( int j = 0; j < mesh->mNumVertices; j++ ) {
            aiVector3t<float> vec = mesh->mVertices[j];
            vertices.push_back( glm::vec3( vec.x, vec.y, vec.z ) );
            colours.push_back( glm::vec3( 1.0f, 1.0f, 1.0f ) );

            aiVector3t<float> nom = mesh->mNormals[j];

            normals.push_back( glm::vec3( nom.x, nom.y, nom.z ) );
        }
    }

    InitializeVAO();
    Load();
}

Geometry::Geometry( std::vector<glm::vec3> v, std::vector<glm::vec3> c, std::vector<glm::vec3> n, GLenum r )
    : vertexBuffer( 0 )
    , textureBuffer( 0 )
    , colourBuffer( 0 )
    , normalBuffer( 0 )
    , vertexArray( 0 )
    , renderMode( r )
    , vertices( std::move( v ) )
    , colours( std::move( c ) )
    , normals( std::move( n ) )
    , ModelMatrix( glm::mat4( 1.0f ) ) {
    InitializeVAO();
    Load();
}

Geometry::Geometry( const CorkTriMesh& trimesh, GLenum r )
    : vertexBuffer( 0 )
    , textureBuffer( 0 )
    , colourBuffer( 0 )
    , normalBuffer( 0 )
    , vertexArray( 0 )
    , renderMode( r )
    , ModelMatrix( glm::mat4( 1.0f ) ) {

    vertices.clear();
    colours.clear();
    normals.clear();

    for ( int i = 0; i < trimesh.n_triangles; ++i ) {
        // Determine points on each face
        glm::vec3 a(
            trimesh.vertices[trimesh.triangles[3 * i]],
            trimesh.vertices[trimesh.triangles[3 * i] + 1],
            trimesh.vertices[trimesh.triangles[3 * i] + 2]
        );

        glm::vec3 b(
            trimesh.vertices[trimesh.triangles[3 * i + 1]],
            trimesh.vertices[trimesh.triangles[3 * i + 1] + 1],
            trimesh.vertices[trimesh.triangles[3 * i + 1] + 2]
        );

        glm::vec3 c(
            trimesh.vertices[trimesh.triangles[3 * i + 2]],
            trimesh.vertices[trimesh.triangles[3 * i + 2] + 1],
            trimesh.vertices[trimesh.triangles[3 * i + 2] + 2]
        );

        vertices.push_back( a );
        vertices.push_back( b );
        vertices.push_back( c );

        // Calculate normals
        glm::vec3 l = a - b;
        glm::vec3 r = b - c;

        // Push colours and normals
        for ( int j = 0; j < 3; ++j ) {
            colours.push_back( glm::vec3( 1.0f, 0.f, 0.f ) );
            normals.push_back( glm::normalize( glm::cross( l, r ) ) );
        }
    }

    InitializeVAO();
    Load();
}

void Geometry::InitializeVAO() {
    // Generate Vertex Buffer Objects
    // Create an array buffer object for storing our vertices
    glGenBuffers( 1, &vertexBuffer );

    // Create another one for storing our colours
    glGenBuffers( 1, &colourBuffer );

    glGenBuffers( 1, &normalBuffer );

    // Set up Vertex Array Object
    // Create a vertex array object encapsulating all our vertex attributes
    glGenVertexArrays( 1, &vertexArray );
    glBindVertexArray( vertexArray );

    // Associate the position array with the vertex array object
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    glVertexAttribPointer(
        VERTEX_INDEX,       //Attribute index
        3,                  //# of components
        GL_FLOAT,           //Type of component
        GL_FALSE,           //Should be normalized?
        0,      //Stride - can use 0 if tightly packed
        0 );                //Offset to first element
    glEnableVertexAttribArray( VERTEX_INDEX );

    // Associate the colour array with the vertex array object
    glBindBuffer( GL_ARRAY_BUFFER, colourBuffer );
    glVertexAttribPointer(
        COLOUR_INDEX,       //Attribute index
        3,                  //# of components
        GL_FLOAT,           //Type of component
        GL_FALSE,           //Should be normalized?
        0,      //Stride - can use 0 if tightly packed
        0 );                //Offset to first element
    glEnableVertexAttribArray( COLOUR_INDEX );

    glBindBuffer( GL_ARRAY_BUFFER, normalBuffer );
    glVertexAttribPointer(
        NORMAL_INDEX,       //Attribute index
        3,                  //# of components
        GL_FLOAT,           //Type of component
        GL_TRUE,           //Should be normalized?
        0,      //Stride - can use 0 if tightly packed
        0 );                //Offset to first element
    glEnableVertexAttribArray( NORMAL_INDEX );

    // Unbind our buffers, resetting to default state
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void Geometry::Load() const {
    // create an array buffer object for storing our vertices
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * vertices.size(), &vertices.front(), GL_STATIC_DRAW );

    // create another one for storing our colours
    glBindBuffer( GL_ARRAY_BUFFER, colourBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * colours.size(), &colours.front(), GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, normalBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * normals.size(), &normals.front(), GL_STATIC_DRAW );

    //Unbind buffer to reset to default state
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void Geometry::ToggleSelectedGeometry() {
    selected = !selected;
}

bool Geometry::IsSelectedGeometry() {
    return selected;
}


// Deallocate geometry-related objects
void Geometry::Destroy() const {
    // Destroy our vertex array object and associated buffers
    glDeleteVertexArrays( 1, &vertexArray );
    glDeleteBuffers( 1, &vertexBuffer );
    glDeleteBuffers( 1, &colourBuffer );
    glDeleteBuffers( 1, &normalBuffer );
}

void Geometry::GetCorkTriMesh( CorkTriMesh& out ) {
    // Dynamically create copies of vertices
    std::vector<glm::vec3>* verts = new std::vector<glm::vec3>( vertices );

    out.n_vertices = verts->size();
    out.vertices = &verts->begin()->x;

    // Index triangles
    std::vector<uint>* triangles = new std::vector<uint>();
    out.n_triangles = verts->size() / 3;

    for ( int i = 0; i < out.n_triangles; ++i ) {
        triangles->push_back( 3 * i );
        triangles->push_back( 3 * i + 1 );
        triangles->push_back( 3 * i + 2 );
    }

    out.triangles = &triangles->front();
}
