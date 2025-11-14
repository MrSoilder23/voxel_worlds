#include "./systems/stone_generation.hpp"

void StoneGenerationSystem::update(bismuth::Registry& registry) {
    constexpr float chunkCoords = VoxelWorlds::CHUNK_SIZE-1.0f;
    auto chunkView       = registry.getView<ChunkStorageComponent,   PositionComponent>();
    auto chunkHeightView = registry.getView<ChunkHeightMapComponent, PositionComponent>();
    std::unordered_map<glm::ivec2, ChunkHeightMapComponent*, IVec2Hash> heightMaps;

    for(auto [entity, heightMap, position] : chunkHeightView) {
        heightMaps.emplace(glm::ivec2(position.position.x, position.position.z), &heightMap);
    }


    for(auto [entity, storage, position] : chunkView) {
        if(storage.wasGenerated) {
            continue;
        }

        auto heightMapIt = heightMaps.find(glm::ivec2(position.position.x, position.position.z));
        if(heightMapIt == heightMaps.end()) {
            continue;
        }
        auto& heightMap = heightMapIt->second;

        for(int blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
            for(int blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {

                int height = heightMap->heightMap[static_cast<size_t>(blockX) * VoxelWorlds::CHUNK_SIZE + static_cast<size_t>(blockZ)];

                int numChunks = static_cast<int>(height / VoxelWorlds::CHUNK_SIZE);
                int remainder = height % static_cast<int>(VoxelWorlds::CHUNK_SIZE); // Extra blocks for the top chunk

                int blocksToPlace = ((position.position.y / VoxelWorlds::CHUNK_SIZE) < numChunks) ? VoxelWorlds::CHUNK_SIZE : 0;
                if((position.position.y / VoxelWorlds::CHUNK_SIZE) == numChunks) {
                    blocksToPlace = remainder;
                }

                for(float blockY = 0; blockY < blocksToPlace; blockY++) {
                    ChunkStorage::InsertToChunk(storage, BlockTypes::stone_block, blockX, blockY, blockZ);
                }
            }
        }

        storage.wasGenerated = true;
    }
}