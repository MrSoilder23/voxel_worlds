#pragma once
// C++ standard libraries
#include <tuple>
#include <unordered_map>
#include <memory>

// Own libraries
#include "chunk.hpp"
#include "model.hpp"
#include "tuple_hash.hpp"
#include "constant.hpp" 
#include "utility.hpp"
#include "chunk_manager.hpp"
#include "entity_manager.hpp"
#include "model_component.hpp"
#include "chunk_renderer_system.hpp"

using ChunkKey = std::tuple<int, int, int>;

class World {
    public:
        void CreateChunk(int chunkX, int chunkY, int chunkZ);
        std::shared_ptr<Chunk> GetChunk(int chunkX, int chunkY, int chunkZ) const;

        BlockTypes GetBlock(int chunkX, int chunkY, int chunkZ, int x, int y, int z);

        void CreateChunkModel(int chunkX, int chunkY, int chunkZ);
        
        void SetCameraPosition(glm::vec3 cameraPosition);
        void SetRenderDistance(float renderDistance);
        void SetSeed(unsigned int seed);

        void GenerateWorld();
        void DrawChunks();

    private:
        std::unordered_map<ChunkKey, std::shared_ptr<Chunk>, Tuple3DHash> chunks;
        
        glm::vec3 mCameraPosition;

        unsigned int mSeed;
        float mRenderDistance;
};