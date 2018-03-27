#include "RayTracer.h"
#include <cmath>

Ray::Ray(glm::vec3 position, glm::vec3 direction): 
pos(position)
{
	dir = glm::normalize(direction);
}

RayTracer::RayTracer(Camera* camera):cam(camera){}

// assumes float x, y is between [-1,1] -- > 0,1
void RayTracer::CastRay(float x, float y){
	
	glm::vec3 cameraPosition = cam->GetPosition();
	glm::vec3 v = glm::normalize(cam->Up());
	glm::vec3 n = glm::normalize(cam->LookAt());
	glm::vec3 u = glm::cross(v, n);
	
	// should receive value from camera
	float fov = 45.0f * (3.141592653589793 / 180.0);
	float aspect = 1.0f;
	
	float height = tan(fov);
	float width = height*aspect;
	
	glm::vec3 po = u*(x*width) + v*(y*height) + cameraPosition + n;

	Ray r(po, po- cameraPosition);
	
}

#define EPS 1E-5

glm::vec3 RayTracer::GetIntersection(Ray ray, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2){
	glm::vec3 triNormal = glm::normalize(glm::cross(p1-p0, p2-p0));
	// t = -(P-Q)*n/(R*n)
	float t = -100;
	float denom = glm::dot(ray.dir,triNormal);
	if(denom != 0){ // ray and triangle normal aren't perpendicular
		t = glm::dot((p0-ray.pos),triNormal)/denom;
		
		glm::vec3 intersect = ray.pos + (ray.dir*t);
		
		// check if it hits the triangle,
		float areaOfTriangle = 0.5 * glm::length(glm::cross(p1-p0, p2-p0));
		//Barycentric coordinates
		float u = 0.5* glm::length(glm::cross(p1-p0, intersect-p0));
		float v = 0.5* glm::length(glm::cross(p2-p0, intersect-p0));
		float w = 0.5* glm::length(glm::cross(p1-p2, intersect-p2));
		
		if(u >= 0.0 && v >= 0.0 && w >=0 &&
		   u+v+w <= areaOfTriangle+EPS &&
		   u+v+w >= areaOfTriangle-EPS
		   ){
			return intersect;
		}
		
	}
	return glm::vec3(-100,-100,-100);
}




