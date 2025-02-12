#pragma once
// C++ standard libraries
#include <memory>
#include <iostream>

// Own libraries
#include "./blocks/block_types.hpp"
#include "chunk.hpp"
#include "model.hpp"
#include "./blocks/block_texture_creator.hpp"
#include "./utility/constant.hpp" 

using uint = unsigned int;

class ChunkManager {
    public:
        void InsertToChunk(Chunk& chunk, BlockTypes block, uint blockX, uint blockY, uint blockZ);

        BlockTypes GetBlock(Chunk& chunk, uint blockX, uint blockY, uint blockZ) const;

        void AddModel(Chunk& chunk, Model model);

        void CreateVAO(Chunk& chunk);
};