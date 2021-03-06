#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>



Camera::Camera( glm::vec3 position, glm::vec3 fwd, glm::vec3 head, glm::vec2 windowRes ):
    pos( position )
    , target( fwd )
    , up( head )
    , fov( 45 ) {

    ViewMatrix = glm::lookAt (
                     pos, // Camera position in World Space
                     target, // and looks at the origin
                     up  // Head is up (set to 0,-1,0 to look upside-down)
                 );

    screen = windowRes;

    //perspective (T const &fovy, T const &aspect, T const &near, T const &far)
    ProjectionMatrix = glm::perspective( glm::radians( fov ), GetAspectRatio(), near, far );
}

void Camera::UpdateViewMatrix() {
    ViewMatrix = glm::lookAt (
                     pos,
                     target,
                     up
                 );
}

void Camera::RotateAround( float deltaX, float deltaY ) {
    // look left and right
    glm::mat4 yaw = glm::rotate( deltaX, up );
    glm::vec4 fwd = yaw * glm::vec4( target - pos, 0 );

    target = glm::vec3( fwd[0], fwd[1], fwd[2] );
    // look up and down
    glm::vec3 bi = glm::normalize( glm::cross( up, ( target - pos ) ) );
    glm::mat4 roll = glm::rotate( -deltaY, bi );

    fwd = ( roll * ( glm::vec4( target - pos, 0 ) ) );
    target = glm::vec3( fwd[0], fwd[1], fwd[2] );

    UpdateViewMatrix();
}


void Camera::Zoom( double yoffset ) {
    const float zoomSpeed = 0.2;

    glm::vec3 direction = zoomSpeed * ( glm::normalize( target - pos ) );

    if ( yoffset != 0 ) {
        direction *= yoffset;
    }

    pos = pos + direction;

    UpdateViewMatrix();
}

void Camera::Panning( float deltaX, float deltaY ) {
    const float panningSpeed = 0.1;
    glm::vec3 bi = glm::normalize( glm::cross( target - pos, up ) );

    if ( deltaX > 0 ) {
        pos = pos - panningSpeed * bi;
    } else if ( deltaX < 0 ) {
        pos = pos + panningSpeed * bi;
    }

    glm::vec3 updir = glm::normalize( glm::cross( bi, target - pos ) );

    if ( deltaY > 0 ) {
        pos = pos + panningSpeed * updir;
    } else if ( deltaY < 0 ) {
        pos = pos - panningSpeed * updir;
    }

    UpdateViewMatrix();
}

glm::vec3 Camera::GetPosition() const {
    return pos;
}
glm::vec3 Camera::Up() const {
    return up;
}

glm::vec3 Camera::LookAtDirection() const {
    return glm::normalize( target - pos );
}

float Camera::GetFov() const {
    return fov;
}

void Camera::SetScreenResolution( const glm::vec2 s ) {
    screen = s;
    ProjectionMatrix = glm::perspective( glm::radians( fov ), GetAspectRatio(), near, far );
}

glm::vec2 Camera::GetScreenResolution() const {
    return screen;
}

float Camera::GetAspectRatio() const {
    return ( float ) screen.x / ( float ) screen.y;
}
