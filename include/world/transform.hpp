#pragma once
// Third_party libraries
#include <glm/glm.hpp>

struct Transform {
    glm::mat4 mModelMatrix{glm::mat4(1.0f)};
};