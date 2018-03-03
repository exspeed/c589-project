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

void scroll_callback( GLFWwindow* window, double xoffset, double yoffset ) {
    if ( inputManager == nullptr ) {
        std::cout << "WHOOPS: inputManager is NULL\n";
        return;
    } else {
        inputManager->ScrollWheel( xoffset, yoffset ); //forward input
    }
}

// PROGRAM ENTRY POINT
int main( int argc, char* argv[] ) {

    // Initialize OpenGL and creat the window
    GLFWwindow* window = nullptr;
    Initialize( window );

    glfwSetScrollCallback( window, scroll_callback );
    Camera* camera = new Camera( glm::vec3( 4, 3, 3 ), glm::vec3( 0, 0, 0 ), glm::vec3( 0, 1, 0 ) );

    // Create Geometry
    Geometry* geometry = new Geometry( "models/cube/cube.obj", GL_TRIANGLES );

    // Create Camera
    Shader program( "shaders/vertex.glsl", "shaders/fragment.glsl" );
    Shader programOutline( "shaders/vertex.glsl", "shaders/outline.frag" );
    Scene* scene = new Scene( &program, &programOutline, camera );
    scene->AddGeometry( geometry );

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
