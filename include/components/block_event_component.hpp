#pragma once
// C++ standard libraries
#include <chrono>

// Third_party libraries
#include <glm/glm.hpp>

struct BlockEventComponent {
    glm::vec3 position;
    std::chrono::system_clock::time_point timestamp;
};