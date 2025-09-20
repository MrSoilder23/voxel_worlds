#include "./systems/world_generation_system.hpp"

WorldGenerationSystem::WorldGenerationSystem(unsigned int seed) : mSeed(seed) {}

void WorldGenerationSystem::update(bismuth::Registry& registry) {
    auto playerView = registry.getView<PlayerTagComponent, PositionComponent>();
    if (playerView.begin() == playerView.end()) {
        return;
    }
    
    auto [playerEntity, player, position] = *playerView.begin();
    
    findChunksToGenerate(position.position);
    
    const int maxChunksPerFrame = 5;
    int chunksGenerated = 0;
    
    while (!mChunksToGenerate.empty() && chunksGenerated < maxChunksPerFrame) {
        glm::ivec3 chunkCoord = mChunksToGenerate.back();
        mChunksToGenerate.pop_back();
        
        generateChunk(registry, chunkCoord.x, chunkCoord.y, chunkCoord.z);
        chunksGenerated++;
    }

}

void WorldGenerationSystem::findChunksToGenerate(const glm::vec3& playerPosition) {
    int playerChunkX = static_cast<int>(std::floor(playerPosition.x / VoxelWorlds::CHUNK_SIZE));
    int playerChunkY = static_cast<int>(std::floor(playerPosition.y / VoxelWorlds::CHUNK_SIZE));
    int playerChunkZ = static_cast<int>(std::floor(playerPosition.z / VoxelWorlds::CHUNK_SIZE));
    
    int renderDistanceChunks = static_cast<int>(mRenderDistance / VoxelWorlds::CHUNK_SIZE);
    
    for (int x = playerChunkX - renderDistanceChunks; x <= playerChunkX + renderDistanceChunks; x++) {
        for (int y = playerChunkY - 1; y <= playerChunkY + 1; y++) { // Limit vertical range
            for (int z = playerChunkZ - renderDistanceChunks; z <= playerChunkZ + renderDistanceChunks; z++) {
                glm::ivec3 chunkCoord(x, y, z);
                
                if (mChunkEntities.find(chunkCoord) != mChunkEntities.end()) {
                    continue;
                }
                
                if (std::find(mChunksToGenerate.begin(), mChunksToGenerate.end(), chunkCoord) != mChunksToGenerate.end()) {
                    continue;
                }
                
                mChunksToGenerate.push_back(chunkCoord);
            }
        }
    }
}



void WorldGenerationSystem::generateChunk(bismuth::Registry& registry, int x, int y, int z) {
    glm::ivec3 chunkCoord = {x, y, z};

    bismuth::EntityID entity = registry.createEntity();
    mChunkEntities[chunkCoord] = entity;
    
    BoundingBoxComponent bBoxComponent;
    bBoxComponent.group = Group::render;
    bBoxComponent.localMin = glm::vec3(-0.5f, -0.5f, -0.5f);
    bBoxComponent.localMax = glm::vec3(
        VoxelWorlds::CHUNK_SIZE - 0.5f,
        VoxelWorlds::CHUNK_SIZE - 0.5f,
        VoxelWorlds::CHUNK_SIZE - 0.5f
    );
    registry.emplaceComponent<BoundingBoxComponent>(entity, bBoxComponent);
    
    PositionComponent posComponent;
    utility::MovePosition(posComponent, glm::vec3(
        x * VoxelWorlds::CHUNK_SIZE,
        y * VoxelWorlds::CHUNK_SIZE,
        z * VoxelWorlds::CHUNK_SIZE
    ));
    registry.emplaceComponent<PositionComponent>(entity, posComponent);
    
    registry.emplaceComponent<ChunkStorageComponent>(entity);
    registry.emplaceComponent<ChunkTagComponent>(entity);
    registry.emplaceComponent<BoundingBoxCollectionComponent>(entity);
    registry.emplaceComponent<ChunkStateComponent>(entity);
    registry.emplaceComponent<MeshComponent>(entity);
    registry.emplaceComponent<MaterialComponent>(entity);
    
    generateNoise(registry, entity, chunkCoord);

}

float WorldGenerationSystem::generateHeight(int x, int z) {
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
void WorldGenerationSystem::generateNoise(
    bismuth::Registry& registry,
    bismuth::EntityID  entity,
    glm::ivec3  const& chunkCoord
) {
    int x = chunkCoord.x;
    int y = chunkCoord.y;
    int z = chunkCoord.z;

    auto& chunkStorage = registry.getComponentPool<ChunkStorageComponent>().getComponent(entity);
    float heightMap[WorldGeneration::CHUNK_SIZE][WorldGeneration::CHUNK_SIZE];

    for(int blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
        for(int blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {
            int globalX = x * VoxelWorlds::CHUNK_SIZE + blockX;
            int globalZ = z * VoxelWorlds::CHUNK_SIZE + blockZ;
            heightMap[blockX][blockZ] = generateHeight(globalX, globalZ);
        }
    }

    static float chunkCoords = VoxelWorlds::CHUNK_SIZE-1.0f;

    // Perlin chunk size
    const int chunkCoordinateX = static_cast<int>(std::floor(static_cast<float>(x)/VoxelWorlds::PERLIN_SCALE));
    const int chunkCoordinateZ = static_cast<int>(std::floor(static_cast<float>(z)/VoxelWorlds::PERLIN_SCALE));

    const int xOffset = (x % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;
    const int zOffset = (z % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;

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
}