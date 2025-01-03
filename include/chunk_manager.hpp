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
#include "tuple_hash.hpp"
#include "component.hpp"
#include "position_component.hpp"
#include "model_component.hpp"
#include "utility.hpp"

using ChunkKey = std::tuple<int, int, int>;
using BlockData = std::unordered_map<std::type_index, std::shared_ptr<IComponent>>;

class ChunkManager {
    public:
        ~ChunkManager();

        std::shared_ptr<Chunk>& CreateChunk(int x, int y, int z);
        std::shared_ptr<Chunk> GetChunk(int x, int y, int z);
        
        void InsertToChunk(std::shared_ptr<Chunk>& chunk, BlockData& block, int x, int y, int z);
        void InitializeChunk(int x, int y, int z);

        BlockData GetBlock(std::shared_ptr<Chunk>& chunk, int x, int y, int z);

        static ChunkManager& GetInstance();

    private:
        std::unordered_map<ChunkKey, std::shared_ptr<Chunk>, Tuple3DHash> chunks;
};