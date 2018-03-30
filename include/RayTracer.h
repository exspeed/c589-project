#include "Scene.h"
#include "Geometry.h"
#include <glm/glm.hpp>

class Ray {
public:
    Ray( glm::vec3 position, glm::vec3 direction );
    glm::vec3 pos, dir;

};

class RayTracer {
    Camera* cam;
public:
    RayTracer( Camera* cam );
    Ray CastRay( float x, float y );
    float GetIntersection( Ray r, glm::vec3,  glm::vec3,  glm::vec3, glm::vec3 );

private:
    // u,v cordinates of an image
    // n direction into the screen
    glm::vec3 u, v, n;

};
