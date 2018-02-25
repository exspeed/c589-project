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
	if( shiftKey && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &xpos, &ypos);
		float deltaX = (xpos - cursorX) * mouseSensitivity;
		float deltaY = (ypos - cursorY) * mouseSensitivity;
		if(!(deltaX || deltaY))
			return;
		camera->Panning(deltaX, deltaY);
	}
	else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
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
	else if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		shiftKey = true;
	}
	else if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		shiftKey = false;
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

void InputManager::ScrollWheel(double xoffset, double yoffset)
{
	if(yoffset == 1 )
	{
		camera->Zoom(true);
	}
	else if (yoffset == -1)
	{
		camera->Zoom(false);
	}

}



