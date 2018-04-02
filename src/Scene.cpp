#include <glad/glad.h>
#include <cassert>
#include "Scene.h"
#include "Camera.h"
#include "RayTracer.h"

Scene::Scene( Camera* cam ):
    camera( cam ) {

}

void Scene::AddGeometry( Geometry* g ) {
    geometries.push_back( g );
}

void Scene::AddSketch( Geometry* g ) {
    sketch = g;
}

void Scene::ClearGeometries() {
    for ( auto geometry : geometries ) {
        delete geometry;
    }

    geometries.clear();
}

void Scene::ClearSketch() {
    SketchConfirmed = false;
    sketch->vertices.clear();
    sketch->colours.clear();
    sketch->normals.clear();
    sketch->Load();
}

int Scene::GetGeometriesSize() {
    return geometries.size();
}

Geometry* Scene::GetGeometry( int i ) {
    return geometries[i];
}

Geometry* Scene::GetSketch() {
    return sketch;
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
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_STENCIL_TEST );

    // Draw non stencil objects here
    for ( int i = 0; i < ( int ) geometries.size(); i++ ) {
        geometries[i]->IsSelectedGeometry()
        ? RenderStencil( geometries[i] )
        : RenderGeometry( geometries[i] );
    }

    RenderSketch( sketch );

    glDisable( GL_DEPTH_TEST );
    glDisable( GL_STENCIL_TEST );
    // reset state to default (no shader or geometry bound)
    glBindVertexArray( 0 );
    glUseProgram( 0 );
}

void Scene::RenderGeometry( Geometry* geometry ) const {
    Shader* program = geometry->program;

    program->use();
    glStencilMask( 0x00 );
    program->setMat4( "Model", geometry->ModelMatrix );
    program->setMat4( "View", camera->ViewMatrix );
    program->setMat4( "Projection", camera->ProjectionMatrix );


    glBindVertexArray( geometry->vertexArray );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, geometry->faceBuffer );
    glDrawElements(
        geometry->renderMode,      // mode
        geometry->faces.size(),    // count
        GL_UNSIGNED_INT,   // type
        ( void* )0         // element array buffer offset
    );
}

void Scene::RenderSketch( Geometry* sketch ) const {
    glDisable(GL_DEPTH_TEST);
    Shader* program = sketch->program;

    program->use();
    program->setMat4( "Model", sketch->ModelMatrix );
    if(SketchConfirmed){
        program->setMat4( "View", camera->ViewMatrix );
        program->setMat4( "Projection", camera->ProjectionMatrix );
    }else{
        program->setMat4( "View", glm::mat4( 1.0f ) );
        program->setMat4( "Projection", glm::mat4( 1.0f ) );
    }

    glBindVertexArray( sketch->vertexArray );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, sketch->vertexBuffer );
    glDrawArrays(
        sketch->renderMode,
        0,
        sketch->vertices.size()
    );
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
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, geometry->faceBuffer );
    glDrawElements(
        GL_TRIANGLES,      // mode
        geometry->faces.size(),    // count
        GL_UNSIGNED_INT,   // type
        ( void* )0         // element array buffer offset
    );

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
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, geometry->faceBuffer );
    glDrawElements(
        GL_TRIANGLES,      // mode
        geometry->faces.size(),    // count
        GL_UNSIGNED_INT,   // type
        ( void* )0         // element array buffer offset
    );

    glBindVertexArray( 0 );
    glStencilMask( 0xFF );
    glClear( GL_STENCIL_BUFFER_BIT );
}

bool Scene::IsSketchConfirmed(){
    return SketchConfirmed; 
}

void Scene::Carve(Geometry* g ){
    RayTracer tracer(camera);

    assert(sketch->normals.size() == sketch->vertices.size());

    std::vector<glm::vec3> sk_vertices;
    std::vector<glm::vec3> sk_normals;
    std::vector<glm::vec3> sk_color;

    for(int i = 0 ; i < (int)sketch->vertices.size(); i++){
        float x = sketch->vertices[i].x; 
        float y = sketch->vertices[i].y; 
        Ray r = tracer.CastRay(x,y);
        float t_min = 10000000;
        glm::vec3 normal;

        for(int j = 0; j < (int)g->faces.size(); j+=3){
            int id0 = g->faces[j];
            int id1 = g->faces[j+1];
            int id2 = g->faces[j+2];

            glm::vec3 n0 = g->normals[id0]; 
            glm::vec3 n1 = g->normals[id1]; 
            glm::vec3 n2 = g->normals[id2]; 

            //face normal 
            glm::vec3 no = glm::normalize(glm::cross(n1-n0, n2-n0));

            glm::vec3 p0 = glm::vec3(g->ModelMatrix*glm::vec4(g->vertices[id0],1));
            glm::vec3 p1 = glm::vec3(g->ModelMatrix*glm::vec4(g->vertices[id1],1));// g->vertices[id1];
            glm::vec3 p2 = glm::vec3(g->ModelMatrix*glm::vec4(g->vertices[id2],1));// g->vertices[id2];


            float t = tracer.GetIntersection(r, p0, p1, p2, no);
            if(t > 0 && t < t_min){
                t_min = t;
                normal = no;
            }
        }

        glm::vec3 inter(r.pos + r.dir*t_min); 
        if(t_min != 10000000){
            sk_vertices.push_back(inter);
            sk_normals.push_back(normal);
            sk_color.emplace_back(1.0f, 0.0f, 1.0f);
        }
    }

    sketch->vertices = sk_vertices;
    sketch->normals = sk_normals;
    sketch->colours = sk_color;
    SketchConfirmed = true; 
    sketch->Load();
}