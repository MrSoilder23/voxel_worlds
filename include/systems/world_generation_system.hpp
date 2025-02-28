#pragma once
// C++ standard libraries
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <mutex>

// Third_party libraries
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Own libraries
#include "./core/entity_manager.hpp"
#include "./components/model_component.hpp"
#include "./components/position_component.hpp"
#include "./components/chunk_model_component.hpp"
#include "./components/chunk_storage_component.hpp"
#include "./utility/utility.hpp"
#include "./blocks/block_registry.hpp"
#include "./blocks/block_texture_creator.hpp"

class WorldGenerationSystem {
    public:
        void SetEntityManager(EntityManager& entityManager);
        void SetSeed(unsigned int seed);

        void GenerateChunk(int x, int y, int z);
        void GenerateModel(int x, int y, int z);

    private:
        void GeneratePerlin(int x, int y, int z, std::string chunkName);
        float GetHeightMultiplier(float continentalness);
        bool CheckBlock(ChunkStorageComponent& currentChunkData, int chunkX, int chunkY, int chunkZ, int x, int y, int z);
    
    private:
        unsigned int mSeed;
        float mRenderDistance;
        std::mutex mWorldGenLock;

        EntityManager* mEntityManager;
};