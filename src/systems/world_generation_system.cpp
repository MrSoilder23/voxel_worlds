#include "./systems/world_generation_system.hpp"

void WorldGenerationSystem::SetEntityManager(EntityManager& entityManager) {
    mEntityManager = &entityManager;
}
void WorldGenerationSystem::SetSeed(unsigned int seed) {
    mSeed = seed;
}
void WorldGenerationSystem::GenerateChunk(int x, int y, int z) {
    glm::ivec3 chunkName = {x, y, z};

    if(!mEntityManager->CreateEntity(chunkName)) {
        return;
    }
    
    BoundingBoxComponent bBoxComponent;
    bBoxComponent.group = Group::render;
    bBoxComponent.mLocalMin = glm::vec3(-0.5f,-0.5f,-0.5f);
    bBoxComponent.mLocalMax = glm::vec3(VoxelWorlds::CHUNK_SIZE-0.5f,
                                   VoxelWorlds::CHUNK_SIZE-0.5f,
                                   VoxelWorlds::CHUNK_SIZE-0.5f);
    
    PositionComponent posComponent;
    utility::MovePosition(posComponent, glm::vec3(x*VoxelWorlds::CHUNK_SIZE,
        y*VoxelWorlds::CHUNK_SIZE, z*VoxelWorlds::CHUNK_SIZE));

    mEntityManager->AddComponent<PositionComponent>(chunkName, posComponent);
    mEntityManager->AddComponent<ChunkStorageComponent>(chunkName);
    mEntityManager->AddComponent<ChunkModelComponent>(chunkName);
    mEntityManager->AddComponent<BoundingBoxComponent>(chunkName, bBoxComponent);
    mEntityManager->AddComponent<BoundingBoxCollectionComponent>(chunkName);
    mEntityManager->AddComponent<ChunkStateComponent>(chunkName);
}

float WorldGenerationSystem::GenerateHeight(int x, int z) {
    x = x + 1343;
    z = z + 343;
    float continentalness = open_simplex_noise::LayeredNoise2D(
        x * (0.005f / (4 * VoxelWorlds::SCALE)),
        z * (0.005f / (4 * VoxelWorlds::SCALE)),
        mSeed,
        4,
        0.5f,
        2.5f
    );
    float erosion = open_simplex_noise::LayeredNoise2D(
        x * (0.004f / (4 * VoxelWorlds::SCALE)),
        z * (0.004f / (4 * VoxelWorlds::SCALE)),
        mSeed,
        4,
        0.4f,
        2.4f
    );
    float peaksAndValleys = open_simplex_noise::LayeredNoise2D(
        x * (0.025f / (10 * VoxelWorlds::SCALE)),
        z * (0.025f / (10 * VoxelWorlds::SCALE)),
        mSeed,
        6,
        0.4f,
        1.8f
    );

    float continentalAdj = VoxelWorlds::CONTINENTAL_SPLINE.evaluate(continentalness);
    float erosionAdj = VoxelWorlds::EROSION_SPLINE.evaluate(erosion);
    float peaksValleysAdj = VoxelWorlds::PEAKS_VALLEYS_SPLINE.evaluate(peaksAndValleys);

    return (continentalAdj * 0.5f) + (erosionAdj * 0.3f) + (peaksValleysAdj * 0.2f);
}

// Private functions
void WorldGenerationSystem::GenerateNoise(float (&heightMap)[WorldGeneration::CHUNK_SIZE][WorldGeneration::CHUNK_SIZE], int x, int y, int z) {
    glm::ivec3 chunkName = {x,y,z};

    auto chunkData = mEntityManager->GetComponent<ChunkStorageComponent>(chunkName);
    static float chunkCoords = VoxelWorlds::CHUNK_SIZE-1.0f;

    // Perlin chunk size
    const int chunkCoordinateX = static_cast<int>(std::floor(static_cast<float>(x)/VoxelWorlds::PERLIN_SCALE));
    const int chunkCoordinateZ = static_cast<int>(std::floor(static_cast<float>(z)/VoxelWorlds::PERLIN_SCALE));

    const int xOffset = (x % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;
    const int zOffset = (z % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;

    ChunkStorageComponent chunkStorage;
    chunkStorage = *chunkData;

    for(float blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
        for(float blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {
            
            float perlin = perlin_noise::LayeredNoise2D(
                chunkCoordinateX,
                chunkCoordinateZ,
                (blockX+(xOffset*chunkCoords))/(chunkCoords*VoxelWorlds::PERLIN_SCALE),
                (blockZ+(zOffset*chunkCoords))/(chunkCoords*VoxelWorlds::PERLIN_SCALE),
                mSeed,
                3,
                VoxelWorlds::PERSISTANCE + 0.3f,
                VoxelWorlds::LACUNARITY + 2
            );

            float height = heightMap[static_cast<size_t>(blockX)][static_cast<size_t>(blockZ)];

            height = std::round(height);

            int numChunks = static_cast<int>(height / VoxelWorlds::CHUNK_SIZE);
            int remainder = static_cast<int>(height) % static_cast<int>(VoxelWorlds::CHUNK_SIZE); // Extra blocks for the top chunk

            int blocksToPlace = (y < numChunks) ? VoxelWorlds::CHUNK_SIZE : 0;
            if(y == numChunks) {
                blocksToPlace = remainder;
            }

            for(float blockY = 0; blockY < blocksToPlace; blockY++) {

                int globalY = (y * VoxelWorlds::CHUNK_SIZE) + blockY;

                if(globalY == height-1) {
                    if(globalY < 100.0f) {
                        if(perlin >= 0.0f && perlin <= 0.6f) {
                            ChunkStorage::InsertToChunk(chunkStorage, BlockTypes::dirt_block, blockX, blockY, blockZ);
                        } else {
                            ChunkStorage::InsertToChunk(chunkStorage, BlockTypes::sand_block, blockX, blockY, blockZ);
                        }
         
                    } else if(globalY < 110.0f) {
                        ChunkStorage::InsertToChunk(chunkStorage, BlockTypes::sand_block, blockX, blockY, blockZ);
                    } else {
                        ChunkStorage::InsertToChunk(chunkStorage, BlockTypes::grass_block, blockX, blockY, blockZ);
                    }

                } else if(globalY <= height-2 && globalY >= height-5){
                    ChunkStorage::InsertToChunk(chunkStorage, BlockTypes::dirt_block, blockX, blockY, blockZ);
                } else {
                    ChunkStorage::InsertToChunk(chunkStorage, BlockTypes::stone_block, blockX, blockY, blockZ);
                }
            }
        }
    }
    chunkStorage.mWasGenerated = true;
    *chunkData = std::move(chunkStorage);
}