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

Camera camera;
bool g_cursorLocked = GL_TRUE;
float g_cursorX = 0;
float g_cursorY = 0;

void windowMouseButtonFunc(GLFWwindow *window, int button, int action,
                           int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            g_cursorLocked = GL_TRUE;
        } else {
            g_cursorLocked = GL_FALSE;
        }
    }
}

void windowMouseMotionFunc(GLFWwindow *window, double x, double y) {
    if (g_cursorLocked) {
        float deltaX = (x - g_cursorX) * 0.01;
        float deltaY = (y - g_cursorY) * 0.01;
        camera.rotateAroundFocus(deltaX, deltaY);
    }
     
    g_cursorX = x;
    g_cursorY = y;
}


// PROGRAM ENTRY POINT
int main(int argc, char *argv[])
{

	// Initialize OpenGL and creat the window
	GLFWwindow* window = nullptr;
	Initialize(window);

  glfwSetCursorPosCallback(window, windowMouseMotionFunc);
  glfwSetMouseButtonCallback(window, windowMouseButtonFunc);

	// Create Geometry
	Geometry* geometry = new Geometry("models/cube/cube.obj", GL_TRIANGLES);

	// Create Camera
	// Create Scene
	Scene scene("shaders/vertex.glsl", "shaders/fragment.glsl", &camera);
	scene.AddGeometry(geometry);

	CheckGLErrors();

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// call function to draw our scene
		scene.Render();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// Clean up allocated resources before exit
	scene.ClearGeometries();
	glUseProgram(0);
	glfwDestroyWindow(window);
	glfwTerminate();

	std::cout << "Goodbye!" << std::endl;
	return 0;
}
