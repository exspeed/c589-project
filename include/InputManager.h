#pragma once

#include "Camera.h"
#include "Scene.h"

class InputManager
{
public:
	GLFWwindow* window;
	Camera* camera;
	Scene* scene;
	
	InputManager(GLFWwindow *window, Camera* cam, Scene* s);
	void CheckInput();
	
private:
	float cursorX = 0;
	float cursorY = 0;
	const float mouseSensitivity = 0.005;
	
	void KeyInput();
 	void MouseInput();
	
};
