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
#include "Scene.h"
#include "Shader.h"
#include "InputManager.h"

static InputManager* inputManager = nullptr;

void KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    assert( inputManager != nullptr );
    inputManager->KeyInput( key, action );
}

void ScrollCallback( GLFWwindow* window, double xoffset, double yoffset ) {
    assert( inputManager != nullptr );
    inputManager->ScrollWheel( xoffset, yoffset ); //forward input
}

// PROGRAM ENTRY POINT
int main( int argc, char* argv[] ) {

    // Initialize OpenGL and creat the window
    GLFWwindow* window = nullptr;
    Initialize( window );

    glfwSetScrollCallback( window, ScrollCallback );
    glfwSetKeyCallback( window, KeyCallback );
    Camera* camera = new Camera( glm::vec3( 4, 3, 3 ), glm::vec3( 0, 0, 0 ), glm::vec3( 0, 1, 0 ) );

    // Create Geometry

    // Create Camera
    Shader* program = new Shader( "shaders/vertex.glsl", "shaders/fragment.glsl" );
    Shader* programOutline = new Shader( "shaders/vertex.glsl", "shaders/outline.frag" );
    Shader* programLine = new Shader( "shaders/vertex.glsl", "shaders/linefrag.frag");

    Geometry* geometry = new Geometry( "models/cube/cube.obj", GL_TRIANGLES, program, programOutline );
    // sketching geometry
    Geometry* sketch = new Geometry( {}, {}, {}, GL_LINE_STRIP, programLine, programOutline );

    Scene* scene = new Scene( camera );
    scene->AddGeometry( geometry );
    scene->AddGeometry( sketch );

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
    delete( inputManager );
    glfwDestroyWindow( window );
    glfwTerminate();

    std::cout << "Goodbye!" << std::endl;
    return 0;
}
