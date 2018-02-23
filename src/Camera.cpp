#include "Camera.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>

Camera::Camera()
	: ProjectionMatrix(glm::mat4(1.0))
	, ViewMatrix(glm::mat4(1.0))
{
	// Initial position
	pos = glm::vec3(4,3,3);
	forward = glm::vec3(0,0,0);
	up = glm::vec3(0,1,0);

	// Initial Field of View
	float initialFoV = 45.0f;

	ViewMatrix = glm::lookAt(
		pos, // Camera position in World Space
		forward, // and looks at the origin
		up  // Head is up (set to 0,-1,0 to look upside-down)
	);

	//perspective (T const &fovy, T const &aspect, T const &near, T const &far)
	ProjectionMatrix = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
}

void Camera::UpdateViewMatrix()
{
	ViewMatrix = glm::lookAt(
		pos,
		forward,
		up
	);
}


void Camera::RotateAround(float deltaX, float deltaY)
{
	// look left and right
	glm::mat4 yaw = glm::rotate(deltaX *0.5f, up);
	forward = yaw*(glm::vec4(forward-pos, 0));

	// look up and down
	glm::vec3 bi = normalize(cross(up, (forward-pos)));
	glm::mat4 roll = glm::rotate(-deltaY *0.5f, bi);

	forward = roll*(glm::vec4(forward-pos, 0));
	
	//m_up = roll*(glm::vec4(m_up, 0));

	UpdateViewMatrix();
}


