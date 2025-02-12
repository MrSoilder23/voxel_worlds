#include "./blocks/block_registry.hpp"

void BlockRegistry::RegisterBlock(BlockTypes type, BlockTemplate block) {
    blocks[type] = std::move(block);
}
BlockTemplate BlockRegistry::GetBlock(BlockTypes type) const {
    return blocks.at(type);
}

BlockRegistry& BlockRegistry::GetInstance() {
    static BlockRegistry sInstance;
    return sInstance;
}