#pragma once
// Third_party libraries
#include <glm/glm.hpp>

// Own libraries
#include "component.hpp"

class PositionComponent : public IComponent {
    public:
        float x,y,z;
};