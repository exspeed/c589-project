#pragma once

#include "Camera.h"
#include <GLFW/glfw3.h>

class InputManager
{
public:
	GLFWwindow* window;
	Camera* camera;
	
	InputManager(GLFWwindow *window, Camera* cam);
	void ScrollWheel(double xoffset, double yoffset);
	void CheckInput();
	
private:
	float cursorX = 0;
	float cursorY = 0;
	bool shiftKey = 0;
	const float mouseSensitivity = 0.005;
	
	void KeyInput();
 	void MouseInput();
	
};
