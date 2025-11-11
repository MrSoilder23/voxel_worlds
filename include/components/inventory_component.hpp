#pragma once
// C++ standard libraries
#include <array>

// Own libraries
#include "blocks/block_types.hpp"

struct ItemSlot {
    BlockTypes item;
    unsigned int count = 1;
    unsigned int maxAmount = 1;
};

struct InventoryComponent {
    std::array<ItemSlot, 10> inventory;
    uint8_t currentSlot = 0;
};