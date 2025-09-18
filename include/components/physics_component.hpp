#pragma once
// Third_party libraries
#include <glm/glm.hpp>

struct PhysicsComponent {
    glm::vec3 velocity = glm::vec3(0.0f);
    
    float mass     = 0.0f;
    float friction = 1.2f;
};