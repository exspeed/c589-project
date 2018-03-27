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
#include "RayTracer.h"

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
	std::vector<glm::vec3> vert;
	float start = -1.0;
	float end = 1.0;
	vert.push_back(glm::vec3(start,-0.1,0));
	vert.push_back(glm::vec3(end,-0.1, 0));
	
	std::vector<glm::vec3> color;
	vert.push_back(glm::vec3(1,1,1));
	vert.push_back(glm::vec3(1,1,1));

    Geometry* geometry = new Geometry( "models/cube/cube.obj", GL_TRIANGLES );
	Geometry* fracture = new Geometry( vert, color, GL_LINES );

    // Create Camera
    Shader program( "shaders/vertex.glsl", "shaders/fragment.glsl" );
	Shader programLine("shaders/line.vert","shaders/line.frag");
    Shader programOutline( "shaders/vertex.glsl", "shaders/outline.frag" );
    Scene* scene = new Scene( &program, &programOutline, camera );
    scene->AddGeometry( geometry );


	RayTracer tracer(camera);
	for(float i = start; i< end; i+=0.1){
		Ray r = tracer.CastRay( i, -0.1);
		float t_min = 1000000;
		for(int j = 0; j < geometry->vertices.size(); j+=3){
			glm::mat4 M = geometry->ModelMatrix;
			glm::mat4 V = camera->ViewMatrix;
			glm::mat4 P = camera->ProjectionMatrix;
			
			glm::mat4 MVP = glm::mat4( 1.0f );
			glm::vec4 a = MVP *glm::vec4(geometry->vertices[j], 1);
			glm::vec4 b = MVP *glm::vec4(geometry->vertices[j+1],1);
			glm::vec4 c = MVP *glm::vec4(geometry->vertices[j+2],1);
			
			float t = tracer.GetIntersection(r, glm::vec3(a[0],a[1],a[2]),glm::vec3(b[0],b[1],b[2]), glm::vec3(c[0],c[1],c[2]));
			if(t > 0 && t < t_min){
				t_min = t;
				std::cout << a[0] << " " << a[1] << " " << a[2] << std::endl;
				std::cout << b[0] << " " << b[1] << " " << b[2] << std::endl;
				std::cout << c[0] << " " << c[1] << " " << c[2] << std::endl;

			}
			break;
		}
		
		if(t_min >= 1000000){
			std::cout << i << " " << -0.1 << " not hit\n";
			
		}else{
			std::cout << i << " " << -0.1 << " hit\n";
		}

	}
	
    inputManager = new InputManager( window, camera, scene );

    CheckGLErrors();

    // Main Loop
    while ( !glfwWindowShouldClose( window ) ) {
        inputManager->CheckInput();
        // call function to draw our scene
        scene->Render();
		
		programLine.use();
		glBindVertexArray( fracture->vertexArray );
		glDrawArrays( fracture->renderMode, 0, fracture->vertices.size() );

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
