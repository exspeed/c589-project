#include "InputManager.h"

InputManager::InputManager(GLFWwindow *w, Camera* cam)
:window(w)
,camera(cam)
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
	else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		camera->Zoom(true);
	}
	else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera->Zoom(false);
	}
}

void InputManager::ScrollWheel(double xoffset, double yoffset){
    if(yoffset == 1 ){
			camera->Zoom(true);
		}
    else if (yoffset == -1){
			camera->Zoom(false);
    }
}



