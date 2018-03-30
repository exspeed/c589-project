#include "RayTracer.h"
#include "GLHelpers.h"
#include <cmath>

Ray::Ray(glm::vec3 position, glm::vec3 direction): 
pos(position)
{
	dir = glm::normalize(direction);
}

RayTracer::RayTracer(Camera* camera):cam(camera){}

// assumes float x, y is between [-1,1]
Ray RayTracer::CastRay(float x, float y){
	x = -x;
	y = y;
	glm::vec3 camPos = cam->GetPosition();
	glm::vec3 forward = glm::normalize(cam->LookAtDirection());
	glm::vec3 up = glm::normalize(cam->Up());
	glm::vec3 right = glm::normalize(glm::cross(up,forward));

	float aspectratio = 1;
	float fov = 45.0f * M_PI/180.0;
	float h = tan(fov);
	float w = h *aspectratio;
	glm::vec3 direction = glm::normalize(x*right*w + y*up*h + forward);

	return Ray(camPos, direction );
	
}

#define EPS 1E-5
#include <iostream>
#include "GLHelpers.h"
float RayTracer::GetIntersection(Ray ray, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2){
	glm::vec3 triNormal = glm::normalize(glm::cross(p1-p0, p2-p0));
	// t = -(P-Q)*n/(R*n)
	float t = 0;
	std::cout << "Triangle Normal "; PrintVec3(triNormal);
	float denom = glm::dot(ray.dir,triNormal);
	if(denom != 0){ // ray and triangle normal aren't perpendicular
		t = glm::dot((p2-ray.pos),triNormal)/denom;
		std::cout << "t: " << t << std::endl;
		if(t < 0){
			std::cout << "Behind " << t << std::endl;
			return -1;
		}
		glm::vec3 intersect = ray.pos + (ray.dir*t);
		
		glm::vec3 inter = ray.pos + ray.dir*t;
		std::cout << inter[0] << " " << inter[1] << " " << inter[2] << std::endl;
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
			std::cout << "IS IN\n";
			return t;
		}
		
		std::cout << "IS OUT\n";
	}
	return -1; // did not intersect
}




