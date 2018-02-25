#include "InputManager.h"

InputManager::InputManager(GLFWwindow *w, Camera* cam, Scene* s)
:window(w)
,camera(cam)
,scene(s)
{
}

void InputManager::CheckInput()
{
	KeyInput();
	MouseInput();
}

void InputManager::MouseInput()
{
	double xpos, ypos;
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Get mouse position
		glfwGetCursorPos(window, &xpos, &ypos);
		float deltaX = (xpos - cursorX) * mouseSensitivity;
		float deltaY = (ypos - cursorY) * mouseSensitivity;
		
		if(!(deltaX || deltaY))
			return;
		camera->RotateAround(deltaX, deltaY);
	}
	
	glfwGetCursorPos(window, &xpos, &ypos);
	cursorX = xpos;
	cursorY = ypos;
	
}
void InputManager::KeyInput()
{
	
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		scene->selected[0] = 1;
	}
}
