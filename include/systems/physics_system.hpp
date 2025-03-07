#pragma once
// Own libraries
#include "./core/entity_manager.hpp"
#include "./components/position_component.hpp"
#include "./components/physics_component.hpp"
#include "./utility/utility.hpp"

class PhysicsSystem {
    public:
        void UpdatePosition(EntityManager& entityManager, std::string entityName, float deltaTime);  
};