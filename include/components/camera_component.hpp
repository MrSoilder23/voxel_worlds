#pragma once
// C++ standard libraries
#include <array>

// Third_party libraries
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct CameraComponent {
    float fov;
    unsigned int screenWidth;
    unsigned int screenHeight;

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);

    std::array<glm::vec4, 5> frustumPlanes;
};