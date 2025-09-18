#pragma once
// Third_party libraries
#include <tbb/tbb.h>

// Own libraries
#include "bismuth/registry.hpp"
#include "utility/perlin_noise.hpp"
#include "utility/world_generation.hpp"
#include "components/chunk_storage_component.hpp"
#include "components/position_component.hpp"

class ChunkCreationSystem {
    public:
        ChunkCreationSystem(const unsigned int& seed) : mSeed(seed) {}
        
        void update(bismuth::Registry& registry);

    private:
        unsigned int mSeed;
};