#pragma once
// C++ standard libraries
#include <sstream>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>
#include <mutex>
#include <array>

// Third_party libraries
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Own libraries
#include "bismuth/registry.hpp"
#include "components/core/mesh.hpp"
#include "components/position_component.hpp"
#include "components/chunk_storage_component.hpp"
#include "components/bounding_box_collection_component.hpp"
#include "components/chunk_state_component.hpp"
#include "components/bounding_box_component.hpp"
#include "components/player_tag.hpp"
#include "components/chunk_tag.hpp"
#include "components/chunk_height_map.hpp"
#include "utility/perlin_noise.hpp"
#include "blocks/block_registry.hpp"
#include "blocks/block_texture_creator.hpp"
#include "utility/open_simplex_noise2.hpp"
#include "utility/spline.hpp"
#include "utility/ivec3_hash.hpp"
#include "utility/world_generation.hpp"

namespace WorldGeneration {
    constexpr size_t CHUNK_SIZE = static_cast<size_t>(VoxelWorlds::CHUNK_SIZE);
}

class WorldGenerationSystem {
    public:
        WorldGenerationSystem(unsigned int seed);

        void update(bismuth::Registry& registry);
                
    private:
        void findChunksToGenerate(
            bismuth::Registry& registry,
            glm::vec3   const& playerPosition
        );
        void generateRing(
            bismuth::Registry& registry,
            glm::ivec3 playerChunk,
            int currentDistance
        );

        void generateNoise(
            bismuth::Registry& registry,
            bismuth::EntityID  entity,
            glm::ivec2  const& chunkCoord
        );
        void generateChunk(bismuth::Registry& registry, int x, int y, int z);
        void generateHeight(bismuth::Registry& registry, int x, int z);

    private:
        unsigned int mSeed;
        std::mutex mWorldGenLock;

        std::unordered_set<glm::ivec3, IVec3Hash> mGeneratedChunks;
        std::unordered_set<glm::ivec3, IVec3Hash> mQueuedChunks;
        std::unordered_set<glm::ivec2, IVec2Hash> mGeneratedHeightMaps;
        
        std::queue<glm::ivec3> mChunksToGenerate;
};