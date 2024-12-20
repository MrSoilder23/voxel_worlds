#pragma once
// C++ standard libraries
#include <unordered_map>
#include <tuple>
#include <memory>

// Third_party libraries
#include <glm/glm.hpp>

// Own libraries
#include "entity_manager.hpp"
#include "chunk.hpp"
#include "position_component.hpp"
#include "tuple_hash.hpp"

using ChunkKey = std::tuple<int, int, int>;
using BlockData = std::unordered_map<std::type_index, std::shared_ptr<IComponent>>;

class ChunkManager {
    public:
        std::shared_ptr<Chunk>& CreateChunk(int x, int y, int z);
        std::shared_ptr<Chunk> GetChunk(int x, int y, int z);
        
        void InsertToChunk(std::shared_ptr<Chunk>& chunk, BlockData& block, uint8_t x, uint8_t y, uint8_t z);

    private:
        std::unordered_map<ChunkKey, std::shared_ptr<Chunk>, Tuple3DHash> chunks;
};