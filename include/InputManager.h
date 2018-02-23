#pragma once

#include "Camera.h"
#include <GLFW/glfw3.h>

class InputManager
{
public:
	GLFWwindow* window;
	Camera* camera;
	
	InputManager(GLFWwindow *window, Camera* cam);
	const void CheckInput();
	
private:
	float cursorX = 0;
	float cursorY = 0;
	
	const void KeyInput();
	const void MouseInput();
	
};
