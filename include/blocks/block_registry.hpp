#pragma once
// C++ standard libraries
#include <memory>
#include <unordered_map>

// Own libraries
#include "block_types.hpp"
#include "model.hpp"
#include "./graphics/texture.hpp"

struct BlockTemplate {    
    Model model;
    std::shared_ptr<Texture> textures;
};

class BlockRegistry {
    public:
        void RegisterBlock(BlockTypes type, BlockTemplate block);
        const BlockTemplate& GetBlock(BlockTypes type) const;

        static BlockRegistry& GetInstance();
    private:
        std::unordered_map<BlockTypes, BlockTemplate> blocks;
};