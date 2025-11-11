#pragma once
// Own libraries
#include "blocks/block_types.hpp"
#include "bismuth/registry.hpp"

struct BlockPlaceEventComponent {
    BlockTypes blockPlaced;
    bismuth::EntityID entityID; // Action done by
};