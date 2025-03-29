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
#include "./components/bounding_box_collection_component.hpp"
#include "./utility/perlin_noise.hpp"
#include "./blocks/block_registry.hpp"
#include "./blocks/block_texture_creator.hpp"
#include "./utility/open_simplex_noise2.hpp"
#include "./utility/spline.hpp"

class WorldGenerationSystem {
    public:
        void SetEntityManager(EntityManager& entityManager);
        void SetSeed(unsigned int seed);

        void GenerateChunk(int x, int y, int z);
        void GenerateModel(int x, int y, int z);

    private:
        void GenerateNoise(int x, int y, int z, std::string chunkName);
        float GenerateHeight(int x, int z);

        bool CheckBlock(ChunkStorageComponent& currentChunkData, int chunkX, int chunkY, int chunkZ, int x, int y, int z);

        char* FastIntToString(char* ptr, int value);
        void FastChunkName(char* ptr, int chunkX, int chunkY, int chunkZ);
    
    private:
        unsigned int mSeed;
        float mRenderDistance;
        std::mutex mWorldGenLock;

        EntityManager* mEntityManager;
};