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
#include "bismuth/registry.hpp"
#include "components/core/mesh.hpp"
#include "components/position_component.hpp"
#include "components/chunk_storage_component.hpp"
#include "components/bounding_box_collection_component.hpp"
#include "components/chunk_state_component.hpp"
#include "components/bounding_box_component.hpp"
#include "components/player_tag.hpp"
#include "components/chunk_tag.hpp"
#include "utility/perlin_noise.hpp"
#include "blocks/block_registry.hpp"
#include "blocks/block_texture_creator.hpp"
#include "utility/open_simplex_noise2.hpp"
#include "utility/spline.hpp"
#include "utility/ivec3_hash.hpp"

namespace WorldGeneration {
    constexpr size_t CHUNK_SIZE = static_cast<size_t>(VoxelWorlds::CHUNK_SIZE);
}

class WorldGenerationSystem {
    public:
        WorldGenerationSystem(unsigned int seed);

        void update(bismuth::Registry& registry);
                
    private:
        void findChunksToGenerate(const glm::vec3& playerPosition);
        void generateNoise(
            bismuth::Registry& registry,
            bismuth::EntityID  entity,
            glm::ivec3  const& chunkCoord
        );
        void generateChunk(bismuth::Registry& registry, int x, int y, int z);
        float generateHeight(int x, int z);

    private:
        unsigned int mSeed;
        float mRenderDistance = 10 * VoxelWorlds::CHUNK_SIZE;
        std::mutex mWorldGenLock;

        std::unordered_map<glm::ivec3, bismuth::EntityID, IVec3Hash> mChunkEntities;
        std::vector<glm::ivec3> mChunksToGenerate;
};