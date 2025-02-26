#pragma once
// Third_party libraries
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Own libraries
#include "./core/component.hpp"

struct PositionComponent : public IComponent {    
    glm::vec3 mPosition = glm::vec3(0.0f);
    glm::quat mRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 mScale = glm::vec3(1.0f);
    glm::mat4 mTransform = glm::mat4(1.0f); 

    bool mDirty = true;
};