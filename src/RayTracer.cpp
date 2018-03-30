#include "RayTracer.h"
#include "GLHelpers.h"

Ray::Ray(glm::vec3 position, glm::vec3 direction): 
pos(position)
{
	dir = glm::normalize(direction);
}

RayTracer::RayTracer(Camera* camera):cam(camera){
	v = glm::normalize(cam->Up());
	float fov = cam->GetFov() * M_PI/180;
	n = glm::normalize(cam->LookAtDirection())*(float)(1/(2*std::tan(fov/2.0)));
	u = glm::normalize(glm::cross(n,v));
}

// assumes float x, y is between [-1,1]
Ray RayTracer::CastRay(float x, float y){
	glm::vec3 camera = cam->GetPosition();
	float normx = ((x+1)/2) - 0.5;
	float normy = ((y+1)/2) - 0.5;

	glm::vec3 po = u*normx + v*normy + camera + n;
	return Ray(po, po - camera );
	
}

#define EPS 1E-5
#include <iostream>
#include "GLHelpers.h"
float RayTracer::GetIntersection(Ray ray, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 no){
	glm::vec3 triNormal = glm::normalize(no);

	float t = 0;
	float denom = glm::dot(ray.dir,triNormal);
	if(denom != 0){ // ray and triangle normal aren't perpendicular
		t = glm::dot((p2-ray.pos),triNormal)/denom;
		std::cout << "t: " << t << std::endl;
		if(t < 0){
			std::cout << "Behind " << t << std::endl;
			return -1;
		}
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
			return t;
		}
	}
	return -1; // did not intersect
}




