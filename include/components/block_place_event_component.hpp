#pragma once
// Own libraries
#include "blocks/block_types.hpp"

struct BlockPlaceEventComponent {
    BlockTypes blockPlaced;
    size_t entityID; // Action done by
};