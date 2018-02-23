
#include "Camera.h"


#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

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


	glm::vec3 position = glm::vec3(4,3,3);
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


void Camera::rotateAround(glm::vec3& vec, glm::vec3 const &axis, float radian){
	radian *= 0.5;
	const float sinAngle = std::sin(radian);
	const float cosAngle = std::cos(radian);

	glm::vec3 n(axis);
	glm::normalize(n);

	glm::quat qAxis(sinAngle, n*cosAngle);

	vec = qAxis*vec;
}


void Camera::rotateAroundFocus(float deltaX, float deltaY) {
  glm::vec3 focus = m_pos + m_forward * m_focusDist;
  glm::vec3 diff = m_pos - focus;

  rotateAround(diff, m_up, -deltaX);
  m_forward = -(normalize(diff));

  glm::vec3 bi = glm::cross(m_up , m_forward);
	normalize(bi);

  rotateAround(diff, bi, deltaY);
  rotateAround(m_up, bi, deltaY);
  m_forward = -(normalize(diff));

  m_pos = focus + diff;

	updateViewMatrix();
}


