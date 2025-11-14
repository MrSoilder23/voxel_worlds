#pragma once
// Third_party libraries
#include <tbb/tbb.h>

// Own libraries
#include "bismuth/registry.hpp"
#include "utility/perlin_noise.hpp"
#include "utility/world_generation.hpp"
#include "utility/ivec3_hash.hpp"
#include "components/chunk_storage_component.hpp"
#include "components/position_component.hpp"
#include "components/chunk_height_map.hpp"

class StoneGenerationSystem {
    public:
        StoneGenerationSystem(const unsigned int& seed) : mSeed(seed) {}
        
        void update(bismuth::Registry& registry);

    private:
        unsigned int mSeed;
};