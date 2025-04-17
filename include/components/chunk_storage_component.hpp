#pragma once
// C++ standard libraries
#include <array>
#include <cassert>
// Own libraries
#include "./core/component.hpp"
#include "./utility/constant.hpp"
#include "./blocks/block_types.hpp"

constexpr size_t TOTAL_CHUNK_SIZE = static_cast<size_t>(VoxelWorlds::CHUNK_SIZE*VoxelWorlds::CHUNK_SIZE*VoxelWorlds::CHUNK_SIZE);

struct ChunkStorageComponent : public IComponent {
    std::array<BlockTypes, TOTAL_CHUNK_SIZE> mBlocks;
    bool mWasGenerated = false;
};

namespace ChunkStorage {
    inline size_t GetIndex(const unsigned int x, const unsigned int y, const unsigned int z) {
        constexpr int chunkSize = VoxelWorlds::CHUNK_SIZE;
        constexpr int chunkSizeSquared = chunkSize * chunkSize;

        return x + y * chunkSize + z * chunkSizeSquared;
    }

    // Insert block coordinates from 0
    inline void InsertToChunk(ChunkStorageComponent& chunk, BlockTypes block, const int x, const int y, const int z) {
        assert(x >= 0 && x < VoxelWorlds::CHUNK_SIZE);
        assert(y >= 0 && y < VoxelWorlds::CHUNK_SIZE);
        assert(z >= 0 && z < VoxelWorlds::CHUNK_SIZE);
    
        chunk.mBlocks[GetIndex(x,y,z)] = block;
    }

    inline BlockTypes& GetBlock(ChunkStorageComponent& chunk, const int x, const int y, const int z) {
        assert(x >= 0 && x < VoxelWorlds::CHUNK_SIZE);
        assert(y >= 0 && y < VoxelWorlds::CHUNK_SIZE);
        assert(z >= 0 && z < VoxelWorlds::CHUNK_SIZE);    

        return chunk.mBlocks[GetIndex(x,y,z)];
    }
}