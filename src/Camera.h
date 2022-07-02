#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using glm::vec3;
using glm::mat4;

#define SENSITIVITY 0.05f

class Camera {
public:
    Camera();
    mat4 getViewMatrix();
    void updateDirection(double x, double y);
private:
    vec3 position;
    vec3 direction;
    vec3 up;
    float yaw;
    float pitch;
};
