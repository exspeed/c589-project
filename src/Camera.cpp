#include "Camera.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

Camera::Camera()
	: ProjectionMatrix(glm::mat4(1.0))
	, ViewMatrix(glm::mat4(1.0))
{
	// Initial position
	glm::vec3 position = glm::vec3(4,3,3);
	// Initial Field of View
	float initialFoV = 45.0f;

	ViewMatrix = glm::lookAt(
		position, // Camera position in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	//perspective (T const &fovy, T const &aspect, T const &near, T const &far)
	ProjectionMatrix = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
}


