
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 getMVP(){
    glm::mat4 View = glm::lookAt(

								glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                            );
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.0f);

    glm::mat4 Model = glm::mat4(1.0);
    return Projection * View * Model;
}