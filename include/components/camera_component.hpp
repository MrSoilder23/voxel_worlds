#pragma once
// Own libraries
#include "./core/component.hpp"
// Third_party libraries
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct CameraComponent : public IComponent {
    CameraComponent() : mEye(glm::vec3(0.0f)),
    mUpVector(glm::vec3(0.0f, 1.0f, 0.0f)) {}
    
    glm::mat4 mProjectionMatrix;
    
    glm::vec3 mEye;
    glm::vec3 mUpVector;
};