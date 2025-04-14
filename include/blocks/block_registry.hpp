#pragma once
// C++ standard libraries
#include <memory>
#include <unordered_map>
#include <array>

// Third_party libraries
#include <glm/glm.hpp>

// Own libraries
#include "block_types.hpp"
#include "model.hpp"
#include "./graphics/texture.hpp"

struct BlockTemplate {    
    Model model;

    // Left Right Front Back Top Bottom
    std::array<glm::vec2, 6> textureCoords;
};

class BlockRegistry {
    public:
        void RegisterBlock(BlockTypes type, BlockTemplate block);
        const BlockTemplate& GetBlock(BlockTypes type) const;

        static BlockRegistry& GetInstance();
    private:
        std::unordered_map<BlockTypes, BlockTemplate> blocks;
};