#pragma once
// C++ standard libraries
#include <tuple>
#include <unordered_map>
#include <memory>
#include <mutex>

// Own libraries
#include "./world/chunk.hpp"
#include "model.hpp"
#include "./utility/tuple_hash.hpp"
#include "./utility/constant.hpp" 
#include "./utility/utility.hpp"
#include "./world/chunk_manager.hpp"
#include "./core/entity_manager.hpp"
#include "./blocks/block_registry.hpp"
#include "./systems/chunk_renderer_system.hpp"

using ChunkKey = std::tuple<int, int, int>;

class World {
    public:
        void CreateChunk(int chunkX, int chunkY, int chunkZ);
        std::shared_ptr<Chunk> GetChunk(int chunkX, int chunkY, int chunkZ) const;

        BlockTypes GetBlock(int chunkX, int chunkY, int chunkZ, int x, int y, int z);
        
        void CreateChunkModel(std::shared_ptr<Chunk> chunk, int chunkX, int chunkY, int chunkZ);
        void GenerateMesh(int chunkX, int chunkZ);
        
        void SetCameraPosition(glm::vec3 cameraPosition);
        void SetRenderDistance(float renderDistance);
        void SetSeed(unsigned int seed);
        
        void GenerateChunks(int chunkX, int chunkZ);
        void GenerateWorld(int chunkX, int chunkZ);
        void DrawChunks(EntityManager& entityManager);
        
        void WorldVao(int chunkX, int loochunkZpZ);
        
    private: 
        void GenerateWorldChunk(int coordinatesX,int coordinatesY,int coordinatesZ);
    private:
        std::unordered_map<ChunkKey, std::shared_ptr<Chunk>, Tuple3DHash> chunks;
        std::mutex mWorldMutex;
        
        glm::vec3 mCameraPosition;

        unsigned int mSeed;
        float mRenderDistance;
};