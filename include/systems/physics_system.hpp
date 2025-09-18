#pragma once
// Own libraries
#include "bismuth/registry.hpp"
#include "./components/position_component.hpp"
#include "./components/physics_component.hpp"
#include "./utility/utility.hpp"

class PhysicsSystem {
    public:
        void update(bismuth::Registry& registry, float deltaTime);
};