#include "InputManager.h"

InputManager::InputManager(GLFWwindow *w, Camera* cam)
:window(w)
,camera(cam)
{
}

void InputManager::CheckInput(){
	KeyInput();
	MouseInput();
}

void InputManager::MouseInput() {
	double xpos, ypos;
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		// Get mouse position
		glfwGetCursorPos(window, &xpos, &ypos);
		float deltaX = (xpos - cursorX) * 0.01;
		float deltaY = (ypos - cursorY) * 0.01;
		camera->RotateAround(deltaX, deltaY);
	}
	
	glfwGetCursorPos(window, &xpos, &ypos);
	cursorX = xpos;
	cursorY = ypos;
	
}
void InputManager::KeyInput() {
	
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}
