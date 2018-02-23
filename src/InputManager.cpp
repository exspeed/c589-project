#include "InputManager.h"

InputManager::InputManager(GLFWwindow *w, Camera* cam)
:window(w)
,camera(cam)
{
}

const void InputManager::CheckInput()
{
	KeyInput();
	MouseInput();
}

const void InputManager::MouseInput()
{
	double xpos, ypos;
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Get mouse position
		glfwGetCursorPos(window, &xpos, &ypos);
		float sensitivity = 0.005f;
		float deltaX = (xpos - cursorX) * sensitivity;
		float deltaY = (ypos - cursorY) * sensitivity;
		
		if(!(deltaX || deltaY))
			return;
		camera->RotateAround(deltaX, deltaY);
	}
	
	glfwGetCursorPos(window, &xpos, &ypos);
	cursorX = xpos;
	cursorY = ypos;
	
}
const void InputManager::KeyInput()
{
	
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}
