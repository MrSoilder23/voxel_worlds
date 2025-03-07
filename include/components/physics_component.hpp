#pragma once
// Third_party libraries
#include <glm/glm.hpp>
// Own libraries
#include "./core/component.hpp"

struct PhysicsComponent : public IComponent{
    glm::vec3 mVelocity = glm::vec3(0.0f);
    float mMass = 0.0f;
};