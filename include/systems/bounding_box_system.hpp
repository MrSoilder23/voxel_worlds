#pragma once
// Own libraries
#include "./components/bounding_box_component.hpp"
#include "./components/position_component.hpp"
#include "./components/model_component.hpp"
#include "./utility/physics.hpp"
#include "./utility/constant.hpp"
#include "./core/entity_manager.hpp"

#include <tbb/tbb.h>

class BoundingBoxSystem {
    public:
        void GenerateBoundingBox(EntityManager& entityManager);
        void GenerateBoundingBoxSingle(EntityManager& entityManager, std::string entityName);
};