#include "./systems/chunk_creation_system.hpp"

void ChunkCreationSystem::update(bismuth::Registry& registry) {
    constexpr float chunkCoords = VoxelWorlds::CHUNK_SIZE-1.0f;
    auto chunkView = registry.getView<ChunkStorageComponent, PositionComponent>();

    for(auto [entity, storage, position] : chunkView) {
        float heightMap[static_cast<size_t>(VoxelWorlds::CHUNK_SIZE)][static_cast<size_t>(VoxelWorlds::CHUNK_SIZE)];
        float perlinMap[static_cast<size_t>(VoxelWorlds::CHUNK_SIZE)][static_cast<size_t>(VoxelWorlds::CHUNK_SIZE)];

        float currentX = std::numeric_limits<float>::infinity();
        float currentZ = std::numeric_limits<float>::infinity();

        if(storage.wasGenerated) {
            return;
        }

        float x = position.position.x / VoxelWorlds::CHUNK_SIZE;
        float z = position.position.z / VoxelWorlds::CHUNK_SIZE;

        // Perlin chunk size
        const int chunkCoordinateX = static_cast<int>(std::floor(x/VoxelWorlds::PERLIN_SCALE));
        const int chunkCoordinateZ = static_cast<int>(std::floor(z/VoxelWorlds::PERLIN_SCALE));

        const int xOffset = (static_cast<int>(x) % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;
        const int zOffset = (static_cast<int>(z) % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;

        ChunkStorageComponent tempChunkStorage = storage;

        if(currentX != position.position.x || currentZ != position.position.z) {
            for(int blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
                for(float blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {
                    heightMap[static_cast<size_t>(blockX)][static_cast<size_t>(blockZ)] = world_generation::GenerateHeight(
                        mSeed, 
                        static_cast<int>(blockX + position.position.x),
                        static_cast<int>(blockZ + position.position.z)
                    );
                    perlinMap[static_cast<size_t>(blockX)][static_cast<size_t>(blockZ)] = perlin_noise::LayeredNoise2D(
                        chunkCoordinateX,
                        chunkCoordinateZ,
                        (blockX+(xOffset*chunkCoords))/(chunkCoords*VoxelWorlds::PERLIN_SCALE),
                        (blockZ+(zOffset*chunkCoords))/(chunkCoords*VoxelWorlds::PERLIN_SCALE),
                        mSeed,
                        3,
                        VoxelWorlds::PERSISTANCE + 0.3f,
                        VoxelWorlds::LACUNARITY + 2
                    );
                }
            }
            currentX = position.position.x;
            currentZ = position.position.z;
        }
            
        for(float blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
            for(float blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {

                float perlin = perlinMap[static_cast<size_t>(blockX)][static_cast<size_t>(blockZ)];
                float height = heightMap[static_cast<size_t>(blockX)][static_cast<size_t>(blockZ)];

                int numChunks = static_cast<int>(height / VoxelWorlds::CHUNK_SIZE);
                int remainder = static_cast<int>(height) % static_cast<int>(VoxelWorlds::CHUNK_SIZE); // Extra blocks for the top chunk

                int blocksToPlace = ((position.position.y / VoxelWorlds::CHUNK_SIZE) < numChunks) ? VoxelWorlds::CHUNK_SIZE : 0;
                if((position.position.y / VoxelWorlds::CHUNK_SIZE) == numChunks) {
                    blocksToPlace = remainder;
                }

                for(float blockY = 0; blockY < blocksToPlace; blockY++) {

                    int globalY = position.position.y + blockY;

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

        tempChunkStorage.wasGenerated = true;
        storage = std::move(tempChunkStorage);
    }
}