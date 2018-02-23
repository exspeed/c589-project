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
#include "ShaderTool.h"
#include "InputManager.h"


// PROGRAM ENTRY POINT
int main(int argc, char *argv[])
{

	// Initialize OpenGL and creat the window
	GLFWwindow* window = nullptr;
	Initialize(window);
	Camera* camera = new Camera;
	InputManager inputManager(window, camera);

	// Create Geometry
	Geometry* geometry = new Geometry("models/cube/cube.obj", GL_TRIANGLES);

	// Create Camera
	// Create Scene
	Scene scene("shaders/vertex.glsl", "shaders/fragment.glsl", camera);
	scene.AddGeometry(geometry);

	CheckGLErrors();

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
