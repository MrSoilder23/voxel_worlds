#pragma once
// Third_party libraries
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Own libraries
#include "components/position_component.hpp"
#include "bismuth/registry.hpp"

class PositionUpdateSystem {
    public:
        void update(bismuth::Registry& entityManager);
};