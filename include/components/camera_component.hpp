#pragma once
// C++ standard libraries
#include <array>

// Own libraries
#include "./core/component.hpp"

// Third_party libraries
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct CameraComponent : public IComponent {
    glm::mat4 mProjectionMatrix;
    glm::mat4 mViewMatrix;

    glm::vec3 mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 mViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);

    std::array<glm::vec4, 5> frustumPlanes;
};