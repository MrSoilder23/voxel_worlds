#pragma once
// Own libraries
#include "./components/bounding_box_component.hpp"
#include "./components/position_component.hpp"
#include "./components/model_component.hpp"
#include "./utility/physics.hpp"
#include "./core/entity_manager.hpp"

class BoundingBoxSystem {
    public:
        void GenerateBoundingBox(EntityManager& entityManager);
        void GenerateBoundingBoxSingle(EntityManager& entityManager, std::string entityName);
};