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
//			Jeremy Hart, University of Calgary
//			John Hall, University of Calgary
// Date:    December 2015
// ==========================================================================

#include "Geometry.h"
#include "GLHelpers.h"
#include "Scene.h"
#include "Shader.h"
#include "InputManager.h"


// PROGRAM ENTRY POINT
int main(int argc, char *argv[])
{

	// Initialize OpenGL and creat the window
	GLFWwindow* window = nullptr;
	Initialize(window);

	Camera* camera = new Camera(glm::vec3(4,3,3), glm::vec3(0,0,0), glm::vec3(0,1,0));
	InputManager inputManager(window, camera);

	// Create Geometry
	Geometry* geometry = new Geometry("models/cube/cube.obj", GL_TRIANGLES);

	// Create Camera
	Shader program("shaders/vertex.glsl","shaders/fragment.glsl");
	Shader programOutline("shaders/vertex.glsl","shaders/outline.frag");
	Scene scene(&program, &programOutline, camera);
	scene.AddGeometry(geometry);

	CheckGLErrors();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	
	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		inputManager.CheckInput();
		// call function to draw our scene
		scene.Render();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// Clean up allocated resources before exit
	scene.ClearGeometries();
	delete(camera);
	glUseProgram(0);
	glfwDestroyWindow(window);
	glfwTerminate();

	std::cout << "Goodbye!" << std::endl;
	return 0;
}
