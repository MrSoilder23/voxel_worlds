#pragma once
// C++ standard libraries
#include <array>

// Own libraries
#include "bismuth/registry.hpp"
#include "components/position_component.hpp"
#include "components/bounding_box_collection_component.hpp"
#include "components/bounding_box_component.hpp"
#include "components/physics_component.hpp"
#include "components/player_tag.hpp"
#include "components/chunk_tag.hpp"
#include "utility/constant.hpp"
#include "utility/physics.hpp"

class CollisionSystem {
    public:
        void update(bismuth::Registry& registry, float deltaTime);
};