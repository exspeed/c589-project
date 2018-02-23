#pragma once

#include "Camera.h"
#include <GLFW/glfw3.h>

class InputManager
{
public:
	GLFWwindow* window;
	Camera* camera;
	
	InputManager(GLFWwindow *window, Camera* cam);
	void CheckInput();
	
private:
	float cursorX = 0;
	float cursorY = 0;
	
	void KeyInput();
	void MouseInput();
	
};
