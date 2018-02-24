#include "Camera.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>



Camera::Camera(glm::vec3 p, glm::vec3 f, glm::vec3 u):
	pos(p) 
, forward(f)
, up(u)
{

	ViewMatrix = glm::lookAt
	(
		pos, // Camera position in World Space
		forward, // and looks at the origin
		up  // Head is up (set to 0,-1,0 to look upside-down)
	);

	//perspective (T const &fovy, T const &aspect, T const &near, T const &far)
	float initialFoV = 45.0f;
	float near = 0.1f;
	float far = 100.f;
	float aspect = 1.0;
	ProjectionMatrix = glm::perspective(glm::radians(initialFoV), aspect, near, far);
}

void Camera::UpdateViewMatrix()
{
	ViewMatrix = glm::lookAt
	(
		pos,
		forward,
		up
	);
}

void Camera::RotateAround(float deltaX, float deltaY)
{
	// look left and right
	glm::mat4 yaw = glm::rotate(deltaX, up);
	glm::vec4 fwd = yaw*glm::vec4(forward-pos, 0);

	forward = glm::vec3(fwd[0], fwd[1], fwd[2]); 
	// look up and down
	glm::vec3 bi = normalize(cross(up, (forward-pos)));
	glm::mat4 roll = glm::rotate(-deltaY, bi);

	fwd = (roll*(glm::vec4(forward-pos, 0)));
	forward = glm::vec3(fwd[0], fwd[1], fwd[2]); 
	
	UpdateViewMatrix();
}


void Camera::Zoom(bool in){
	const float zoomSpeed = 0.2;

	glm::vec3 direction = zoomSpeed*(normalize(forward-pos));
	if(!in){
		direction = -direction;
	}
	pos = pos + direction;

	UpdateViewMatrix();
}



