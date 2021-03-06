#include "Camera.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Settings.h"

using glm::normalize;
using glm::cross;
using glm::radians;

float Camera::movementUnits;

vec3 Camera::position; 
vec3 Camera::direction; 
vec3 Camera::up; 

vec3 Camera::focusPoint; 
vec3 Camera::firstPosition; 
bool Camera::shouldFocusCenter = false;

Camera::Camera() {
    position  = vec3(0.0f, 0.0f,  0.0f);
    direction = vec3(0.0f, 0.0f, -1.0f);
    up        = vec3(0.0f, 1.0f,  0.0f);
    movementUnits = DEFAULT_MOVEMENT_UNITS;
}

void Camera::setFocusPoint(vec3 focusPoint, float boundingBoxSide) {
    Camera::focusPoint = focusPoint;
    position.z = (boundingBoxSide/2)/tan(Settings::fieldOfView/2.0);
    firstPosition = position;
    focus();
}

void Camera::updateDirection(float x, float y) {
    if(!shouldFocusCenter){
        vec3 left = cross(direction, up);
        // pitch rotation
        vec3 newDirection = rotate(direction, radians(-y*SENSITIVITY), left);
        // lock pitch into 85 degrees
        if(angle(newDirection, up) > radians(5.0f) 
                && angle(newDirection,-up) > radians(5.0f)) {
            direction = newDirection;
        }
        // yaw rotation
        direction = rotate(direction, radians(-x*SENSITIVITY), up);
    }
}

void Camera::moveFront() {
    position += direction * movementUnits;
}

void Camera::moveBack() {
    position -= direction * movementUnits;
}

void Camera::moveRight() {
    if(shouldFocusCenter) focus();
    vec3 right = cross(-direction, up);
    position -= right * movementUnits;
    if(shouldFocusCenter) focus();
}

void Camera::moveLeft() {
    if(shouldFocusCenter) focus();
    vec3 left = cross(direction, up);
    position -= left * movementUnits;
    if(shouldFocusCenter) focus();

}

void Camera::moveUp() {
    if(shouldFocusCenter) focus();
    vec3 right = cross(-direction, up);
    vec3 upFromCamera = cross(direction, right);
    position += upFromCamera * movementUnits;
    if(shouldFocusCenter) focus();
}

void Camera::moveDown() {
    if(shouldFocusCenter) focus();
    vec3 left = cross(direction, up);
    vec3 downFromCamera = cross(direction, left);
    position += downFromCamera * movementUnits;
    if(shouldFocusCenter) focus();
}

void Camera::center() {
    shouldFocusCenter = not shouldFocusCenter;
    if (shouldFocusCenter) focus();
}

void Camera::reset() {
    position = firstPosition;
    focus();
}

void Camera::focus() {
    direction = normalize(focusPoint - position);
}
