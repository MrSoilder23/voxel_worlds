#pragma once
// C++ standard libraries
#include <array>
#include <stdexcept>

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
        static constexpr size_t NUM_BLOCK_TYPES = static_cast<size_t>(BlockTypes::COUNT);

        void RegisterBlock(BlockTypes type, BlockTemplate block);
        const BlockTemplate& GetBlock(BlockTypes type) const;

        static BlockRegistry& GetInstance();
    private: 
        std::array<BlockTemplate, NUM_BLOCK_TYPES> blocks;

};