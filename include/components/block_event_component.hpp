#pragma once
// Third_party libraries
#include <glm/glm.hpp>

// Own libraries
#include "./core/component.hpp"

struct BlockEventComponent : public IComponent {
    glm::vec3 mPosition;
    std::chrono::system_clock::time_point mTimestamp;
};