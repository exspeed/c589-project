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

#include <iostream>
void InputManager::KeyInput()
{

	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	for(int i = 0; i < scene->selected.size() && i < 9; i++)
	{
		static int oldState = GLFW_RELEASE;
		int newState = glfwGetKey(window, GLFW_KEY_0+i+1);
		if(newState == GLFW_RELEASE && oldState == GLFW_PRESS)
		{
			scene->selected[i] = scene->selected[i] ? 0 : 1;
		}
		oldState = newState;
	}

}
