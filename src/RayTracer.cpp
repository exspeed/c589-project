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
	glm::vec3 camera = cam->GetPosition();
	glm::vec3 v = glm::normalize(cam->Up());

	float fov = 45 * M_PI/180;
	glm::vec3 n = glm::normalize(cam->LookAtDirection())*(float)(1/(2*std::tan(fov/2.0)));
	glm::vec3 u = glm::normalize(glm::cross(n,v));

	float normx = ((x+1)/2) - 0.5;
	float normy = ((y+1)/2) - 0.5;

	glm::vec3 po = u*normx + v*normy + camera + n;
	return Ray(po, po - camera );
	
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




