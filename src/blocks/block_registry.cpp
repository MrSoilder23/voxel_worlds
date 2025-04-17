#include "./blocks/block_registry.hpp"

void BlockRegistry::RegisterBlock(BlockTypes type, BlockTemplate block) {
    const size_t index = static_cast<size_t>(type);
    if (index >= NUM_BLOCK_TYPES) {
        throw std::out_of_range("Invalid BlockType");
    }

    blocks[index] = std::move(block);
}
const BlockTemplate& BlockRegistry::GetBlock(BlockTypes type) const {
    static BlockTemplate defaultBlock;

    const size_t index = static_cast<size_t>(type);
    if (index >= NUM_BLOCK_TYPES) {
        return defaultBlock; // Or throw
    }

    return blocks[index];
}

BlockRegistry& BlockRegistry::GetInstance() {
    static BlockRegistry sInstance;
    return sInstance;
}