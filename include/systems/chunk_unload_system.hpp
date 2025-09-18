#pragma once
// Third_party libraries
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

// Own libraries
#include "bismuth/registry.hpp"
#include "components/position_component.hpp"
#include "components/chunk_tag.hpp"
#include "components/player_tag.hpp"
#include "utility/constant.hpp"

class ChunkUnloadSystem {
    public:
        void update(bismuth::Registry& registry);
};