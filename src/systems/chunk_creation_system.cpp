#include "./systems/chunk_creation_system.hpp"

void ChunkCreationSystem::CreateChunkData(EntityManager& entityManager, const unsigned int& seed) {
    const auto chunkStorages = entityManager.GetComponentArray<ChunkStorageComponent>();
    const auto chunkPositions = entityManager.GetComponentArray<PositionComponent>();

    float heightMap[static_cast<size_t>(VoxelWorlds::CHUNK_SIZE)][static_cast<size_t>(VoxelWorlds::CHUNK_SIZE)];
    float perlinMap[static_cast<size_t>(VoxelWorlds::CHUNK_SIZE)][static_cast<size_t>(VoxelWorlds::CHUNK_SIZE)];

    float currentX = std::numeric_limits<float>::infinity();
    float currentZ = std::numeric_limits<float>::infinity();

    for(size_t entityID = 0; entityID < chunkStorages.size(); entityID++) {
        auto& chunkStorage = chunkStorages[entityID];
        auto& chunkPosition = chunkPositions[entityID];

        if(!chunkStorage || !chunkPosition) {
            continue;
        }

        if(chunkStorage->mWasGenerated) {
            continue;
        }

        static float chunkCoords = VoxelWorlds::CHUNK_SIZE-1.0f;

        float x = chunkPosition->mPosition.x / VoxelWorlds::CHUNK_SIZE;
        float z = chunkPosition->mPosition.z / VoxelWorlds::CHUNK_SIZE;

        // Perlin chunk size
        const int chunkCoordinateX = static_cast<int>(std::floor(x/VoxelWorlds::PERLIN_SCALE));
        const int chunkCoordinateZ = static_cast<int>(std::floor(z/VoxelWorlds::PERLIN_SCALE));

        const int xOffset = (static_cast<int>(x) % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;
        const int zOffset = (static_cast<int>(z) % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;

        ChunkStorageComponent tempChunkStorage = *chunkStorage;

        if(currentX != chunkPosition->mPosition.x || currentZ != chunkPosition->mPosition.z) {
            for(int blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
                for(float blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {
                    heightMap[static_cast<size_t>(blockX)][static_cast<size_t>(blockZ)] = world_generation::GenerateHeight(
                        seed, 
                        static_cast<int>(blockX + chunkPosition->mPosition.x),
                        static_cast<int>(blockZ + chunkPosition->mPosition.z)
                    );
                    perlinMap[static_cast<size_t>(blockX)][static_cast<size_t>(blockZ)] = perlin_noise::LayeredNoise2D(
                        chunkCoordinateX,
                        chunkCoordinateZ,
                        (blockX+(xOffset*chunkCoords))/(chunkCoords*VoxelWorlds::PERLIN_SCALE),
                        (blockZ+(zOffset*chunkCoords))/(chunkCoords*VoxelWorlds::PERLIN_SCALE),
                        seed,
                        3,
                        VoxelWorlds::PERSISTANCE + 0.3f,
                        VoxelWorlds::LACUNARITY + 2
                    );
                }
            }
            currentX = chunkPosition->mPosition.x;
            currentZ = chunkPosition->mPosition.z;
        }
            
        for(float blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
            for(float blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {

                float perlin = perlinMap[static_cast<size_t>(blockX)][static_cast<size_t>(blockZ)];
                float height = heightMap[static_cast<size_t>(blockX)][static_cast<size_t>(blockZ)];

                height = std::round(height);

                int numChunks = static_cast<int>(height / VoxelWorlds::CHUNK_SIZE);
                int remainder = static_cast<int>(height) % static_cast<int>(VoxelWorlds::CHUNK_SIZE); // Extra blocks for the top chunk

                int blocksToPlace = ((chunkPosition->mPosition.y / VoxelWorlds::CHUNK_SIZE) < numChunks) ? VoxelWorlds::CHUNK_SIZE : 0;
                if((chunkPosition->mPosition.y / VoxelWorlds::CHUNK_SIZE) == numChunks) {
                    blocksToPlace = remainder;
                }

                for(float blockY = 0; blockY < blocksToPlace; blockY++) {

                    int globalY = chunkPosition->mPosition.y + blockY;

                    if(globalY == height-1) {
                        if(globalY < 100.0f) {
                            if(perlin >= 0.0f && perlin <= 0.6f) {
                                ChunkStorage::InsertToChunk(tempChunkStorage, BlockTypes::dirt_block, blockX, blockY, blockZ);
                            } else {
                                ChunkStorage::InsertToChunk(tempChunkStorage, BlockTypes::sand_block, blockX, blockY, blockZ);
                            }
            
                        } else if(globalY < 110.0f) {
                            ChunkStorage::InsertToChunk(tempChunkStorage, BlockTypes::sand_block, blockX, blockY, blockZ);
                        } else {
                            ChunkStorage::InsertToChunk(tempChunkStorage, BlockTypes::grass_block, blockX, blockY, blockZ);
                        }

                    } else if(globalY <= height-2 && globalY >= height-5){
                        ChunkStorage::InsertToChunk(tempChunkStorage, BlockTypes::dirt_block, blockX, blockY, blockZ);
                    } else {
                        ChunkStorage::InsertToChunk(tempChunkStorage, BlockTypes::stone_block, blockX, blockY, blockZ);
                    }
                }
            }
        }

        tempChunkStorage.mWasGenerated = true;
        *chunkStorage = std::move(tempChunkStorage);
    }
}