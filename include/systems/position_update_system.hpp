#pragma once
// Third_party libraries
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Own libraries
#include "./components/position_component.hpp"
#include "./core/entity_manager.hpp"

class PositionUpdateSystem {
    public:
        void UpdatePositionTransform(EntityManager& entityManager);
        void UpdatePositionTransformSingle(EntityManager& entityManager, std::string entityName);
};