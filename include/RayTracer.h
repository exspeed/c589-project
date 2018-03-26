#include "Scene.h"
#include "Geometry.h"
#include <glm/glm.hpp>

class Ray {
public:
    Ray(glm::vec3 position, glm::vec3 direction);
    glm::vec3 pos, dir; 

};

class RayTracer{
	Camera* cam;	
public:
	RayTracer(Camera* cam);
	void CastRay(float x, float y);
	glm::vec3 GetIntersection(Ray r, glm::vec3,  glm::vec3,  glm::vec3);
};
