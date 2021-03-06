#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "assimp/postprocess.h"
#include "Geometry.h"
#include <iostream>

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
    , faceBuffer( g.faceBuffer )
    , vertexArray( g.vertexArray )
    , renderMode( g.renderMode )
    , program( g.program )
    , programOutline( g.programOutline )
    , vertices( g.vertices )
    , colours( g.colours )
    , normals( g.normals )
    , faces( g.faces )
    , ModelMatrix( g.ModelMatrix ) {
    InitializeVAO();
    Load();
}

Geometry::Geometry( const std::string filename, GLenum r, Shader* geo, Shader* stencil )
    : vertexBuffer( 0 )
    , textureBuffer( 0 )
    , colourBuffer( 0 )
    , normalBuffer( 0 )
    , faceBuffer( 0 )
    , vertexArray( 0 )
    , renderMode( r )
    , program( geo )
    , programOutline( stencil )
    , vertices( std::vector<glm::vec3>() )
    , colours( std::vector<glm::vec3>() )
    , normals( std::vector<glm::vec3>() )
    , faces( std::vector<GLuint>() )
    , ModelMatrix( glm::mat4( 1.0f ) ) {
    Assimp::Importer importer;
    auto import_flags = aiProcess_FindDegenerates | aiProcess_FindInvalidData |
                        aiProcess_FixInfacingNormals | aiProcess_ImproveCacheLocality |
                        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                        aiProcess_OptimizeMeshes;
    const aiScene* scene = importer.ReadFile( filename, import_flags );

    if ( !scene ) {
        printf( "Unable to load mesh: %s\n", importer.GetErrorString() );
    }

    for ( int i = 0; i < ( int ) scene->mNumMeshes; i++ ) {
        auto mesh = scene->mMeshes[i];

        for ( uint j = 0; j < mesh->mNumVertices; j++ ) {
            aiVector3t<float> vec = mesh->mVertices[j];
            vertices.push_back( glm::vec3( vec.x, vec.y, vec.z ) );
            colours.push_back( glm::vec3( 1.0f, 1.0f, 1.0f ) );

            aiVector3t<float> nom = mesh->mNormals[j];

            normals.push_back( glm::vec3( nom.x, nom.y, nom.z ) );
        }

        // With aiProcess_Triangulate, we are guaranteed that face.mNumIndices = 3
        for ( uint j = 0; j < mesh->mNumFaces; ++j ) {
            aiFace face = mesh->mFaces[j];

            for ( uint k = 0; k < face.mNumIndices; ++k ) {
                faces.push_back( face.mIndices[k] );
            }
        }
    }

    InitializeVAO();
    Load();
}

Geometry::Geometry( std::vector<glm::vec3> v, std::vector<glm::vec3> c, std::vector<glm::vec3> n, GLenum r, Shader* geo, Shader* stencil )
    : vertexBuffer( 0 )
    , textureBuffer( 0 )
    , colourBuffer( 0 )
    , normalBuffer( 0 )
    , faceBuffer( 0 )
    , vertexArray( 0 )
    , renderMode( r )
    , program( geo )
    , programOutline( stencil )
    , vertices( std::move( v ) )
    , colours( std::move( c ) )
    , normals( std::move( n ) )
    , ModelMatrix( glm::mat4( 1.0f ) ) {

    for ( int i = 0; i < vertices.size(); i += 3 ) {
        faces.push_back( i );
        faces.push_back( i + 1 );
        faces.push_back( i + 2 );
    }

    InitializeVAO();
    Load();
}

Geometry::Geometry( const CorkTriMesh& trimesh, GLenum r, Shader* geo, Shader* stencil )
    : vertexBuffer( 0 )
    , textureBuffer( 0 )
    , colourBuffer( 0 )
    , normalBuffer( 0 )
    , faceBuffer( 0 )
    , vertexArray( 0 )
    , renderMode( r )
    , program( geo )
    , programOutline( stencil )
    , ModelMatrix( glm::mat4( 1.0f ) ) {

    vertices.clear();
    colours.clear();
    normals.clear();
    faces.clear();

    // Push vertices
    for ( int i = 0; i < trimesh.n_vertices; ++i ) {
        vertices.emplace_back(
            trimesh.vertices[3 * i],
            trimesh.vertices[3 * i + 1],
            trimesh.vertices[3 * i + 2]
        );
    }

    // Push faces
    for ( int i = 0; i < trimesh.n_triangles; ++i ) {
        faces.emplace_back( trimesh.triangles[3 * i + 0] );
        faces.emplace_back( trimesh.triangles[3 * i + 1] );
        faces.emplace_back( trimesh.triangles[3 * i + 2] );
    }

    colours = std::vector<glm::vec3>( vertices.size(), glm::vec3( 1.f, 1.f, 1.f ) );
    normals = std::vector<glm::vec3>( vertices.size(), glm::vec3( 0.f, 0.f, 0.f ) );

    for ( int i = 0; i < trimesh.n_triangles; ++i ) {
        // Determine points on each face
        glm::vec3 a = vertices[trimesh.triangles[3 * i + 0]];
        glm::vec3 b = vertices[trimesh.triangles[3 * i + 1]];
        glm::vec3 c = vertices[trimesh.triangles[3 * i + 2]];

        // Calculate normals
        glm::vec3 l = b - a;
        glm::vec3 r = c - a;
        glm::vec3 n = glm::cross( l, r );
        // Aggregate normals
        normals[trimesh.triangles[3 * i + 0]] += n;
        normals[trimesh.triangles[3 * i + 1]] += n;
        normals[trimesh.triangles[3 * i + 2]] += n;
    }

    for ( int i = 0; i < normals.size(); ++i ) {
        // Average normals
        normals[i] = glm::normalize( normals[i] );
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

    // Create another one for normals
    glGenBuffers( 1, &normalBuffer );

    // Create another one for faces
    glGenBuffers( 1, &faceBuffer );

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
    // Load vertices
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * vertices.size(), &vertices.front(), GL_STATIC_DRAW );

    // Load colours
    glBindBuffer( GL_ARRAY_BUFFER, colourBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * colours.size(), &colours.front(), GL_STATIC_DRAW );

    // Load normals
    glBindBuffer( GL_ARRAY_BUFFER, normalBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( glm::vec3 ) * normals.size(), &normals.front(), GL_STATIC_DRAW );

    // Load faces
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, faceBuffer );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof( unsigned int ), &faces[0], GL_STATIC_DRAW );

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

void Geometry::Scale( const glm::vec3 scale ) {
    ModelMatrix = glm::scale( ModelMatrix, scale );
}

void Geometry::Translate( const glm::vec3 translate ) {
    ModelMatrix = glm::translate( ModelMatrix, translate );
}

void Geometry::Rotate( const glm::vec3 rotate, const float angle ) {
    ModelMatrix = glm::rotate( ModelMatrix, angle, rotate );
}

// http://www.inesc-id.pt/ficheiros/publicacoes/2502.pdf source/reference paper

void Geometry::SmoothLine() {
    std::vector<glm::vec3> ALPCPs;  // Arc Length Parameterized Control Points
    std::vector<glm::vec3> RCSCPs;  // Reverse Chaikin Subdivision Control Points

    ALPCPs = ArcLengthParameterize( vertices );

    RCSCPs = ALPCPs;
    const int numReverseSubdivisions = 3;

    // reverse chaikin subdivision 3 times
    for ( int i = 0; i < numReverseSubdivisions; i++ ) {
        RCSCPs = ChaikinReverseSubdivision( RCSCPs );
    }

    // chaikin subdivision 4 times
    const int numSubdivisions = 4;

    for ( int i = 0; i < numSubdivisions; i++ ) {
        RCSCPs = ChaikinSubdivision( RCSCPs );
    }

    vertices = RCSCPs;
    colours.clear();
    normals.clear();
    colours = std::vector<glm::vec3>( vertices.size(), glm::vec3( 1.f, 0.f, 0.f ) );
    normals = std::vector<glm::vec3>( vertices.size(), glm::vec3( 0.f, 1.f, 0.f ) );
}

std::vector<glm::vec3> Geometry::ArcLengthParameterize( std::vector<glm::vec3> vertices ) {
    float length = 0.0;

    // Find the length of the line created by the control points
    for ( int i = 0; i < vertices.size() - 1; i++ ) {
        glm::vec3 pi = vertices[i];
        glm::vec3 pj = vertices[i + 1];
        length += glm::length( pj - pi );
    }

    // Find the nearest power of 2 using log base 2
    int nearestPowerOf2 = floor( log2f( ( float )vertices.size() ) ) + 1;
    int numPoints = pow( 2, nearestPowerOf2 ) - 3; // -3 just makes it so we remove 1 chaikin reverse subdivision point so we have an even # of cps
    float distance = ( float ) length / ( float ) numPoints;

    // subdivide the line many times
    std::vector<glm::vec3> subdividedCPs = vertices;

    for ( int i = 0; i < 5; i++ ) {
        subdividedCPs = Subdivide( subdividedCPs );
    }

    // parameterize the line
    std::vector<glm::vec3> arclengthParameterizedCPs;
    float total = 0.0f;
    glm::vec3 pi;
    glm::vec3 pj;
    float deltad;

    arclengthParameterizedCPs.push_back( subdividedCPs.front() );

    for ( int i = 0; i < subdividedCPs.size() - 1; i++ ) {
        pi = subdividedCPs[i];
        pj = subdividedCPs[i + 1];
        deltad = glm::length( pj - pi );
        total += deltad;

        if ( total >= distance ) {
            arclengthParameterizedCPs.push_back( pj );
            total -= distance;
        }
    }

    // if we don't have enough control points, add the last one
    if ( arclengthParameterizedCPs.size() % 2 != 0 ) {
        arclengthParameterizedCPs.push_back( subdividedCPs.back() );
    }

    return arclengthParameterizedCPs;
}

std::vector<glm::vec3> Geometry::Subdivide( std::vector<glm::vec3> vertices ) {
    std::vector<glm::vec3> subdividedCPs;
    glm::vec3 pi;
    glm::vec3 pj;
    glm::vec3 pij;

    // First point is added
    pi = vertices.front();
    subdividedCPs.push_back( pi );

    for ( int i = 0; i < vertices.size() - 1; i++ ) {
        pi = vertices[i];
        pj = vertices[i + 1];
        pij = 0.5f * pi + 0.5f * pj; // 1/2 pi + 1/2 pj
        subdividedCPs.push_back( pij );
        subdividedCPs.push_back( pj );
    }

    return subdividedCPs;
}

std::vector<glm::vec3> Geometry::ChaikinReverseSubdivision( std::vector<glm::vec3> F ) {
    std::vector<glm::vec3> C;
    C.push_back( F.front() );
    glm::vec3 C1 = -0.5f * F[0] + F[1] + 0.75f * F[2] - 0.25f * F[3];
    C.push_back( C1 );

    glm::vec3 Cj;

    for ( int i = 2; i <= ( int ) F.size() - 6; i += 2 ) {
        Cj = -0.25f * F[i] + 0.75f * F[i + 1] + 0.75f * F[i + 2] - 0.25f * F[i + 3];
        C.push_back( Cj );
    }

    Cj = -0.25f * F[F.size() - 4] + 0.75f * F[F.size() - 3] + F[F.size() - 2] - 0.5f * F.back(); // F.back = f.size()-1
    C.push_back( Cj );
    C.push_back( F.back() );

    return C;
}

std::vector<glm::vec3> Geometry::ChaikinSubdivision( std::vector<glm::vec3> C ) {
    std::vector<glm::vec3> F;
    F.push_back( C.front() );
    glm::vec3 F1 = 0.5f * C[0] + 0.5f * C[1];
    F.push_back( F1 );

    glm::vec3 Fi;
    glm::vec3 Fj;

    for ( int i = 2; i < C.size() - 1; ++i ) {
        Fi = 0.75f * C[i - 1] + 0.25f * C[i];
        Fj = 0.25f * C[i - 1] + 0.75f * C[i];
        F.push_back( Fi );
        F.push_back( Fj );
    }

    Fi = 0.5f * C[C.size() - 2] + 0.5f * C[C.size() - 1];
    F.push_back( Fi );
    F.push_back( C.back() );
    return F;
}

Geometry* Geometry::Crack( Geometry* inp, Geometry* crack ) {
    Geometry* cracked = *inp - *crack;
    cracked->RemoveDegeneracies();
    return cracked;
}

Geometry* Geometry::operator+( Geometry& g ) {
    // Prepare CorkTriMesh
    CorkTriMesh a, b, out;
    GetCorkTriMesh( a );
    g.GetCorkTriMesh( b );

    // Boolean operation
    computeUnion( a, b, &out );

    // Dynamically instantiate resulting geometry
    Geometry* ret = new Geometry( out, renderMode, program, programOutline );

    // Free up memory
    freeCorkTriMesh( &a );
    freeCorkTriMesh( &b );
    freeCorkTriMesh( &out );

    return ret;
}

Geometry* Geometry::operator-( Geometry& g ) {
    // Prepare CorkTriMesh
    CorkTriMesh a, b, out;
    GetCorkTriMesh( a );
    g.GetCorkTriMesh( b );

    // Boolean operation
    computeDifference( a, b, &out );

    // Dynamically instantiate resulting geometry
    Geometry* ret = new Geometry( out, renderMode, program, programOutline );

    // Free up memory
    freeCorkTriMesh( &a );
    freeCorkTriMesh( &b );
    freeCorkTriMesh( &out );

    return ret;
}

Geometry* Geometry::operator*( Geometry& g ) {
    // Prepare CorkTriMesh
    CorkTriMesh a, b, out;
    GetCorkTriMesh( a );
    g.GetCorkTriMesh( b );

    // Boolean operation
    computeIntersection( a, b, &out );

    // Dynamically instantiate resulting geometry
    Geometry* ret = new Geometry( out, renderMode, program, programOutline );

    // Free up memory
    freeCorkTriMesh( &a );
    freeCorkTriMesh( &b );
    freeCorkTriMesh( &out );

    return ret;
}

Geometry* Geometry::operator/( Geometry& g ) {
    // Prepare CorkTriMesh
    CorkTriMesh a, b, out;
    GetCorkTriMesh( a );
    g.GetCorkTriMesh( b );

    // Boolean operation
    computeSymmetricDifference( a, b, &out );

    // Dynamically instantiate resulting geometry
    Geometry* ret = new Geometry( out, renderMode, program, programOutline );

    // Free up memory
    freeCorkTriMesh( &a );
    freeCorkTriMesh( &b );
    freeCorkTriMesh( &out );

    return ret;
}

void Geometry::GetCorkTriMesh( CorkTriMesh& out ) {
    // Dynamically create copies of geometry data
    std::vector<GLuint>* nfaces = new std::vector<GLuint>( faces );
    std::vector<glm::vec3>* nvertices = new std::vector<glm::vec3>( vertices );

    // Transform vertex positions to world space
    for ( auto& v : *nvertices ) {
        glm::vec4 nv = ModelMatrix * glm::vec4( v, 1.f );
        v = glm::vec3( nv.x, nv.y, nv.z );
    }

    out.n_vertices = nvertices->size();
    out.vertices = &nvertices->front().x;
    out.n_triangles = nfaces->size() / 3;
    out.triangles = &nfaces->front();
}

void Geometry::RemoveDegeneracies() {
    for ( auto i = faces.begin(); i != faces.end() - 3; ) {
        glm::vec3 a = vertices[*i];
        glm::vec3 b = vertices[*( i + 1 )];
        glm::vec3 c = vertices[*( i + 2 )];

        float area =  glm::length( glm::cross( a - b, a - c ) ) * 0.5f;

        if ( area <= 1E-5 ) {
            faces.erase( i );
            faces.erase( i );
            faces.erase( i );
        } else {
            i += 3;
        }
    }

    Load();
}
