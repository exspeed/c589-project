#include <glad/glad.h>
#include "Scene.h"
#include "Camera.h"

Scene::Scene( Camera* cam ):
    camera( cam ) {

}

void Scene::AddGeometry( Geometry* g ) {
    geometries.push_back( g );
}

void Scene::ClearGeometries() {
    for ( auto geometry : geometries ) {
        delete geometry;
    }

    geometries.clear();
}

int Scene::GetGeometriesSize() {
    return geometries.size();
}

Geometry* Scene::GetGeometry( int i ) {
    return geometries[i];
}

void Scene::ToggleSelectedGeometry( int i ) {
    if ( i < 0 || i >= geometries.size() ) {
        return;
    }

    geometries[i]->ToggleSelectedGeometry();
}

bool Scene::HasAnyGeometrySelected() {
    for ( int i  = 0; i < ( int ) geometries.size(); i++ ) {
        Geometry* geometry = geometries[i];

        if ( geometry->IsSelectedGeometry() ) {
            return true;
        }
    }

    return false;
}

// Rendering function that draws our scene to the frame buffer
void Scene::Render() const {
    glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
	glEnable( GL_DEPTH_TEST );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    // Draw non stencil objects here
    for ( int i = 0; i < ( int ) geometries.size(); i++ ) {
        if ( !geometries[i]->IsSelectedGeometry() ) {

            Shader* program = geometries[i]->program;
            program->use();
            glStencilMask( 0x00 );
            program->setMat4( "Model", geometries[i]->ModelMatrix );
            program->setMat4( "View", camera->ViewMatrix );
            program->setMat4( "Projection", camera->ProjectionMatrix );


            glBindVertexArray( geometries[i]->vertexArray );
            glDrawArrays( geometries[i]->renderMode, 0, geometries[i]->vertices.size() );
        } else {

            RenderStencil( geometries[i] );
			glClear(GL_STENCIL_BUFFER_BIT);
        }
    }

    // reset state to default (no shader or geometry bound)
    glBindVertexArray( 0 );
    glUseProgram( 0 );
}


void Scene::RenderStencil( Geometry* geometry ) const {
    Shader* program = geometry->program;
    program->use();
    glStencilFunc( GL_ALWAYS, 1, 0xFF );
    glStencilMask( 0xFF );

    program->setMat4( "Model", geometry->ModelMatrix );
    program->setMat4( "View", camera->ViewMatrix );
    program->setMat4( "Projection", camera->ProjectionMatrix );

    // bind our shader program and the vertex array object containing our
    // scene geometry, then tell OpenGL to draw our geometry
    glBindVertexArray( geometry->vertexArray );
    glDrawArrays( geometry->renderMode, 0, geometry->vertices.size() );

    glStencilFunc( GL_NOTEQUAL, 1, 0xFF );
    glStencilMask( 0x00 );
    glDisable( GL_DEPTH_TEST );


    Shader* programOutline = geometry->programOutline;
    programOutline->use();

    float tenpercentscale = 1.1f;
    glm::mat4 model = glm::scale( geometry->ModelMatrix, glm::vec3( tenpercentscale ) );
    programOutline->setMat4( "Model", model );
    programOutline->setMat4( "View", camera->ViewMatrix );
    programOutline->setMat4( "Projection", camera->ProjectionMatrix );


    glBindVertexArray( geometry->vertexArray );
    glDrawArrays( geometry->renderMode, 0, geometry->vertices.size() );

    glEnable( GL_DEPTH_TEST );

    glBindVertexArray( 0 );
    glStencilMask( 0xFF );
    glClear( GL_STENCIL_BUFFER_BIT );
}
