#pragma once
// C++ standard libraries
#include <array>
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
    inline size_t GetIndex(int x, int y, int z) {
        return x + y * VoxelWorlds::CHUNK_SIZE + z * VoxelWorlds::CHUNK_SIZE * VoxelWorlds::CHUNK_SIZE;
    }

    // Insert block coordinates from 0
    inline void InsertToChunk(ChunkStorageComponent& chunk, BlockTypes block, int x, int y, int z) {
        assert(x >= 0 && x < VoxelWorlds::CHUNK_SIZE);
        assert(y >= 0 && y < VoxelWorlds::CHUNK_SIZE);
        assert(z >= 0 && z < VoxelWorlds::CHUNK_SIZE);
    

        chunk.mBlocks[GetIndex(x,y,z)] = block;
    }

    inline BlockTypes& GetBlock(ChunkStorageComponent& chunk, int x, int y, int z) {
        assert(x >= 0 && x < VoxelWorlds::CHUNK_SIZE);
        assert(y >= 0 && y < VoxelWorlds::CHUNK_SIZE);
        assert(z >= 0 && z < VoxelWorlds::CHUNK_SIZE);    

        return chunk.mBlocks[GetIndex(x,y,z)];
    }
}