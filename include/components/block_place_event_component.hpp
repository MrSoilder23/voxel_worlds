#pragma once
// Own libraries
#include "./core/component.hpp"
#include "./blocks/block_types.hpp"

struct BlockPlaceEventComponent : public IComponent {
    BlockTypes mBlockPlaced;
    size_t mEntityID; // Action done by
};