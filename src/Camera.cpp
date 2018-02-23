
#include "Camera.h"


#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>

void PrintMat4(glm::mat4 const &m);

Camera::Camera()
	: ProjectionMatrix(glm::mat4(1.0))
	, ViewMatrix(glm::mat4(1.0))
{
	// Initial position
	m_pos = glm::vec3(4,3,3);
	m_focusDist = m_pos.length();
	m_forward = glm::vec3(0,0,0);
	m_up = glm::vec3(0,1,0);

	// Initial Field of View
	float initialFoV = 45.0f;

	ViewMatrix = glm::lookAt(
		m_pos, // Camera position in World Space
		m_forward, // and looks at the origin
		m_up  // Head is up (set to 0,-1,0 to look upside-down)
	);

	PrintMat4((glm::mat4)ViewMatrix);
	//perspective (T const &fovy, T const &aspect, T const &near, T const &far)
	ProjectionMatrix = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
}

void Camera::updateViewMatrix(){
	ViewMatrix = glm::lookAt(
		m_pos,
		m_forward,
		m_up
	);
}

#include <iostream>

void Camera::rotateAround(float deltaX, float deltaY) {
	
	// look left and right
	glm::mat4 yaw = glm::rotate(deltaX *0.5f, m_up);
	m_forward = yaw*(glm::vec4(m_forward-m_pos, 0));

	// look up and down
	glm::vec3 bi = normalize(cross(m_up, (m_forward-m_pos)));
	glm::mat4 roll = glm::rotate(-deltaY *0.5f, bi);

	m_forward = roll*(glm::vec4(m_forward-m_pos, 0));
	//m_up = roll*(glm::vec4(m_up, 0));


	updateViewMatrix();
}


