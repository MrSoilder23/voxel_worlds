#pragma once
// Third_party libraries
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

// Own libraries
#include "./core/entity_manager.hpp"
#include "./components/position_component.hpp"
#include "./utility/constant.hpp"

class ChunkUnloadSystem {
    public:
        void UnloadChunks(EntityManager& entityManager);
};