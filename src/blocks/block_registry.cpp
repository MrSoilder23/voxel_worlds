#include "./blocks/block_registry.hpp"

void BlockRegistry::RegisterBlock(BlockTypes type, BlockTemplate block) {
    blocks[type] = std::move(block);
}
const BlockTemplate& BlockRegistry::GetBlock(BlockTypes type) const {
    static BlockTemplate defaultBlock = BlockTemplate();

    auto it = blocks.find(type);
    if(it != blocks.end()) {
        return it->second;
    }

    return defaultBlock;
}

BlockRegistry& BlockRegistry::GetInstance() {
    static BlockRegistry sInstance;
    return sInstance;
}