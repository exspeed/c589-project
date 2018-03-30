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
    Camera* camera = new Camera( glm::vec3( 0, 0, -5.0), glm::vec3( 0, 0, 0 ), glm::vec3( 0, 1, 0 ) );

    // Create Geometry
	std::vector<glm::vec3> vert;
	float start = -0.15f;
	float y = -.8;
	vert.push_back(glm::vec3(start,y,0));
	
	std::vector<glm::vec3> color;
	vert.push_back(glm::vec3(1,1,1));
	vert.push_back(glm::vec3(1,1,1));

    Geometry* geometry = new Geometry( "models/cube/cube.obj", GL_TRIANGLES );

		geometry->vertices[0] = glm::vec3(-1,-1, 0.0);
		geometry->vertices[1] = glm::vec3(-1, 1, 0.0);
		geometry->vertices[2] = glm::vec3(1, -1, 0.0);
		Geometry* fracture = new Geometry( vert, color, {},  GL_POINTS );

    Shader program( "shaders/vertex.glsl", "shaders/fragment.glsl" );
		Shader programLine("shaders/line.vert","shaders/line.frag");
    Shader programOutline( "shaders/vertex.glsl", "shaders/outline.frag" );
    Scene* scene = new Scene( &program, &programOutline, camera );
    scene->AddGeometry( geometry );

	glPointSize(3);
	Geometry* HIT = new Geometry( std::vector<glm::vec3>(), {}, {},  GL_POINTS );

	RayTracer tracer(camera);
	Ray r = tracer.CastRay( start, y);
	std::cout << "Ray pos: ";
	PrintVec3(r.pos);
	std::cout << "Ray dir: ";
	PrintVec3(r.dir);

	float t_min = 1000000;
	glm::mat4 M = geometry->ModelMatrix;
	glm::mat4 V = camera->ViewMatrix;
	glm::mat4 P = camera->ProjectionMatrix;
			
	glm::mat4 MVP = glm::mat4(1.0f);


	glm::vec3 a = glm::vec3(MVP*glm::vec4(geometry->vertices[0],1));
	glm::vec3 b = glm::vec3(MVP*glm::vec4(geometry->vertices[0+1],1));
	glm::vec3 c = glm::vec3(MVP*glm::vec4(geometry->vertices[0+2],1));
	PrintVec3(a);
	PrintVec3(b);
	PrintVec3(c);
	
	float t = tracer.GetIntersection(r, a, b, c);
	if(t > 0 && t < t_min){
		t_min = t;
	}
		
	if(t_min >= 1000000){
		//std::cout << i << " " << -0.1 << " not hit\n";
			
	}else{
		std::cout << " hit\n";
	}

		HIT->Load();	
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

				glBindVertexArray( HIT->vertexArray );
				glDrawArrays( HIT->renderMode, 0, HIT->vertices.size() );

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
