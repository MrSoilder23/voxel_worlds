#pragma once
// Third_party libraries
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Own libraries
#include "./core/component.hpp"

struct PositionComponent : public IComponent {
    PositionComponent() : mViewDirection(glm::vec3(0.0f, 0.0f, -1.0f)) {}
    
    glm::vec3 mViewDirection;
    glm::quat mOrientation;
};