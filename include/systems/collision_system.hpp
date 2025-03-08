#pragma once
// C++ standard libraries
#include <array>

// Own libraries
#include "./core/entity_manager.hpp"
#include "./components/position_component.hpp"
#include "./components/bounding_box_collection_component.hpp"
#include "./components/bounding_box_component.hpp"
#include "./components/physics_component.hpp"
#include "./utility/constant.hpp"
#include "./utility/utility.hpp"

class CollisionSystem {
    public:
        void UpdateCollision(EntityManager& entityManager, float deltaTime);
};