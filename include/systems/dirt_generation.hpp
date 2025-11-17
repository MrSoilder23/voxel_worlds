#pragma once
// C++ standard libraries
#include <tuple>
#include <queue>

// Own libraries
#include "bismuth/registry.hpp"
#include "utility/perlin_noise.hpp"
#include "utility/world_generation.hpp"
#include "utility/ivec3_hash.hpp"
#include "components/chunk_storage_component.hpp"
#include "components/position_component.hpp"
#include "components/chunk_state_component.hpp"

class DirtGenerationSystem {
    public:
        DirtGenerationSystem(const unsigned int& seed) : mSeed(seed) {}
        
        void update(bismuth::Registry& registry);

    private:
        unsigned int mSeed;
};