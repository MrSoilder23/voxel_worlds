#pragma once
// C++ standard libraries
#include <array>

// Own libraries
#include "./core/component.hpp"
#include "./blocks/block_types.hpp"

struct ItemSlot {
    BlockTypes mItem;
    unsigned int mCount = 1;
    unsigned int mMaxAmount = 1;
};

struct InventoryComponent : public IComponent {
    std::array<ItemSlot, 10> mInventory;
    unsigned int mCurrentSlot = 0;
};