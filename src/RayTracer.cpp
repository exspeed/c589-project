#include "RayTracer.h"
#include <cmath>
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays

Ray::Ray(glm::vec3 position, glm::vec3 direction): 
pos(position)
{
	dir = glm::normalize(direction);
}

RayTracer::RayTracer(Camera* camera):cam(camera){}


// it is normalized: double x,y [0,1]
void RayTracer::CastRay(float x, float y){
	glm::vec3 cameraPosition = cam->GetPosition();
	glm::vec3 v = cam->Up();
	float fov = 45.0f * (3.141592653589793 / 180.0);
	glm::vec3 n = cam->LookAt()* (float)(1/(2*tan(fov)));
	glm::vec3 u = glm::normalize(glm::cross(v, n)); 

	// normalize by size of window.
	// double normx = xo/width - 0.5 
	// double normy = yo/height - 0.5 

	float normx = x - 0.5; 
	float normy = y - 0.5; 
	glm::vec3 po = u*normx + v*normy + cameraPosition + n;	

	Ray r(po, po- cameraPosition);

}


