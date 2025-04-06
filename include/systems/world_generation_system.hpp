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
#include "./components/chunk_state_component.hpp"
#include "./utility/perlin_noise.hpp"
#include "./blocks/block_registry.hpp"
#include "./blocks/block_texture_creator.hpp"
#include "./utility/open_simplex_noise2.hpp"
#include "./utility/spline.hpp"

namespace WorldGeneration {
    constexpr size_t CHUNK_SIZE = static_cast<size_t>(VoxelWorlds::CHUNK_SIZE);
}

class WorldGenerationSystem {
    public:
        void SetEntityManager(EntityManager& entityManager);
        void SetSeed(unsigned int seed);

        void GenerateChunk(float (&heightMap)[WorldGeneration::CHUNK_SIZE][WorldGeneration::CHUNK_SIZE], int x, int y, int z);
        
        float GenerateHeight(int x, int z);
    private:
        void GenerateNoise(float (&heightMap)[WorldGeneration::CHUNK_SIZE][WorldGeneration::CHUNK_SIZE], int x, int y, int z);
    private:
        unsigned int mSeed;
        float mRenderDistance;
        std::mutex mWorldGenLock;

        EntityManager* mEntityManager;
};