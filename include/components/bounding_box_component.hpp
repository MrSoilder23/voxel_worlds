#pragma once
// Third_party libraries
#include <glm/glm.hpp>
// Own libraries
#include "group.hpp"

struct BoundingBoxComponent{
    // Corners, with minimum value and maximum value
    glm::vec3 localMin;
    glm::vec3 localMax;
    
    glm::vec3 worldMin;
    glm::vec3 worldMax;

    Group group;
    Group mask;
};