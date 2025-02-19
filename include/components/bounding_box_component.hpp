#pragma once
// Third_party libraries
#include <glm/glm.hpp>
// Own libraries
#include "./core/component.hpp"

struct BoundingBoxComponent : public IComponent{
    // Corners, with minimum value and maximum value
    glm::vec3 mMin;
    glm::vec3 mMax;
};