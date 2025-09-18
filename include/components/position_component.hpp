#pragma once
// Third_party libraries
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct PositionComponent {    
    glm::vec3 position  = glm::vec3(0.0f);
    glm::quat rotation  = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale     = glm::vec3(1.0f);
    glm::mat4 transform = glm::mat4(1.0f); 

    bool dirty = true;
};