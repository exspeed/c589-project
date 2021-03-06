// ==========================================================================
// Barebones OpenGL Core Profile Boilerplate
//    using the GLFW windowing system (http://www.glfw.org)
//
// Loosely based on
//  - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
//  - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)
//
// Author:  Sonny Chan, University of Calgary
// Co-Authors:
//          Jeremy Hart, University of Calgary
//          John Hall, University of Calgary
// Date:    December 2015
// ==========================================================================

#include "Geometry.h"
#include "GLHelpers.h"
#include "InputManager.h"
#include "Scene.h"
#include "Shader.h"

static InputManager* inputManager = nullptr;
Camera* camera;
Geometry* cursor;
Scene* scene;

void KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    assert( inputManager != nullptr );
    inputManager->KeyInput( key, action );
}

void ScrollCallback( GLFWwindow* window, double xoffset, double yoffset ) {
    assert( inputManager != nullptr );
    inputManager->ScrollWheel( xoffset, yoffset ); //forward input
}

void FramebufferCallback( GLFWwindow* window, int width, int height ) {
    glViewport( 0, 0, width, height );

    if ( camera != nullptr ) {
        camera->SetScreenResolution( glm::vec2( ( float ) width, ( float ) height ) );
    }
}

void CursorPositionCallback( GLFWwindow* window, double xpos, double ypos ) {
    if ( cursor == nullptr ) {
        return;
    }

    glm::vec2 screenRes = camera->GetScreenResolution();
    float MVPX = 2.0f * ( xpos / screenRes.x ) - 1;
    float MVPY = -2.0f * ( ypos / screenRes.y ) + 1;

    glPointSize( scene->WIDTH * 2000.f );
    cursor->vertices[0] = glm::vec3( MVPX, MVPY, 0.f );
    cursor->Load();
}

// PROGRAM ENTRY POINT
int main( int argc, char* argv[] ) {
    // Initialize OpenGL and creat the window
    GLFWwindow* window = nullptr;
    Initialize( window );

    glfwSetScrollCallback( window, ScrollCallback );
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, CursorPositionCallback );
    glfwSetFramebufferSizeCallback( window, FramebufferCallback );

    int width, height;
    glfwGetWindowSize( window, &width, &height );
    camera = new Camera( glm::vec3( 4, 3, 3 ), glm::vec3( 0, 0, 0 ), glm::vec3( 0, 1, 0 ),  glm::vec2( width, height ) );

    // Create Camera
    Shader* program = new Shader( "shaders/vertex.glsl", "shaders/fragment.glsl" );
    Shader* programOutline = new Shader( "shaders/vertex.glsl", "shaders/outline.frag" );
    Shader* programLine = new Shader( "shaders/vertex.glsl", "shaders/linefrag.frag" );

    // Initial Mesh
    Geometry* geometry = new Geometry( "models/cube/cube.obj", GL_TRIANGLES, program, programOutline );
    // sketching geometry
    Geometry* sketch = new Geometry( {}, {}, {}, GL_LINE_STRIP, programLine, nullptr );
    cursor = new Geometry( {glm::vec3( 0.f, 0.f, 0.f )}, {glm::vec3( 1.f, 0.f, 0.f )}, {glm::vec3()}, GL_POINTS, programLine, nullptr );

    // Hack in crack pattern (for now)
    Geometry* crack_pattern = new Geometry( "models/cube/cube.obj", GL_TRIANGLES, program, programOutline );
    crack_pattern->Scale( glm::vec3( 0.25f, 0.25f, 0.25f ) );
    crack_pattern->Translate( glm::vec3( 0.f, 2.f, 0.f ) );

    // Crack
    Geometry* cracked = Geometry::Crack( geometry, crack_pattern );

    scene = new Scene( camera );
    scene->AddGeometry( geometry );
    //scene->AddGeometry( cracked );

    scene->AddSketch( sketch );

    scene->AddCursor( cursor );

    inputManager = new InputManager( window, camera, scene );

    CheckGLErrors();

    // Main Loop
    while ( !glfwWindowShouldClose( window ) ) {
        inputManager->CheckInput();
        // call function to draw our scene
        scene->Render();

        glfwSwapBuffers( window );

        glfwPollEvents();
    }

    // Clean up allocated resources before exit
    scene->ClearGeometries();
    delete( camera );
    delete( scene );
    delete( cursor );
    delete( inputManager );
    glfwDestroyWindow( window );
    glfwTerminate();

    std::cout << "Goodbye!" << std::endl;
    return 0;
}
