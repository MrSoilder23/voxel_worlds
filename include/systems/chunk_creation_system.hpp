#pragma once
// Third_party libraries
#include <tbb/tbb.h>

// Own libraries
#include "./core/entity_manager.hpp"
#include "./utility/perlin_noise.hpp"
#include "./utility/world_generation.hpp"
#include "./components/chunk_storage_component.hpp"
#include "./components/position_component.hpp"

class ChunkCreationSystem {
    public:
        void CreateChunkData(EntityManager& entityManager, const unsigned int& seed);
};