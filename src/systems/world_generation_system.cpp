#include "./systems/world_generation_system.hpp"

WorldGenerationSystem::WorldGenerationSystem(unsigned int seed) : mSeed(seed) {}

void WorldGenerationSystem::update(bismuth::Registry& registry) {
    auto playerView = registry.getView<PlayerTagComponent, PositionComponent>();
    if (playerView.begin() == playerView.end()) {
        return;
    }
    
    auto [playerEntity, player, position] = *playerView.begin();
    
    findChunksToGenerate(registry, position.position);
    
    int chunksGenerated = 0;
    
    while (!mChunksToGenerate.empty() && chunksGenerated < VoxelWorlds::CHUNK_GEN_SPEED) {
        glm::ivec3 chunkCoord = mChunksToGenerate.front();
        mChunksToGenerate.pop();
        
        generateChunk(registry, chunkCoord.x, chunkCoord.y, chunkCoord.z);
        chunksGenerated++;
    }

}

void WorldGenerationSystem::findChunksToGenerate(
    bismuth::Registry& registry,
    glm::vec3   const& playerPosition
) {
    using VoxelWorlds::CHUNK_GENERATION_OFFSET;

    static int RENDER_DISTANCE_CHUNK = VoxelWorlds::RENDER_DISTANCE;

    int playerChunkX = static_cast<int>(std::floor(playerPosition.x / VoxelWorlds::CHUNK_SIZE));
    int playerChunkY = static_cast<int>(std::floor(playerPosition.y / VoxelWorlds::CHUNK_SIZE));
    int playerChunkZ = static_cast<int>(std::floor(playerPosition.z / VoxelWorlds::CHUNK_SIZE));
    
    for (int x = playerChunkX - RENDER_DISTANCE_CHUNK - CHUNK_GENERATION_OFFSET; x <= playerChunkX + RENDER_DISTANCE_CHUNK + CHUNK_GENERATION_OFFSET; x++) {
        for (int y = playerChunkY - RENDER_DISTANCE_CHUNK - CHUNK_GENERATION_OFFSET; y <= playerChunkY + RENDER_DISTANCE_CHUNK + CHUNK_GENERATION_OFFSET; y++) {
            for (int z = playerChunkZ - RENDER_DISTANCE_CHUNK - CHUNK_GENERATION_OFFSET; z <= playerChunkZ + RENDER_DISTANCE_CHUNK + CHUNK_GENERATION_OFFSET; z++) {
                glm::ivec3 chunkCoord(x, y, z);
                
                if (mGeneratedChunks.contains(chunkCoord) || 
                    mQueuedChunks.contains(chunkCoord)) {
                    continue;
                }
                
                mChunksToGenerate.push(chunkCoord);
                mQueuedChunks.insert(chunkCoord);

                if(!mGeneratedHeightMaps.contains(glm::ivec2(x,z))) {
                    generateHeight(registry, x, z);
                }

            }
        }
    }
}



void WorldGenerationSystem::generateChunk(bismuth::Registry& registry, int x, int y, int z) {
    glm::ivec3 chunkCoord = {x, y, z};

    bismuth::EntityID entity = registry.createEntity();
    
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
    
    mGeneratedChunks.insert(chunkCoord);
    mQueuedChunks.erase(chunkCoord);
}

void WorldGenerationSystem::generateHeight(bismuth::Registry& registry, int x, int z) {
    bismuth::EntityID entity = registry.createEntity();
    glm::ivec2 chunkCoord(x, z);

    PositionComponent posComponent;
    utility::MovePosition(posComponent, glm::vec3(
        x * VoxelWorlds::CHUNK_SIZE,
        0,
        z * VoxelWorlds::CHUNK_SIZE
    ));

    registry.emplaceComponent<ChunkHeightMapComponent>(entity);
    registry.emplaceComponent<PositionComponent>(entity, posComponent);

    generateNoise(registry, entity, chunkCoord);

    mGeneratedHeightMaps.insert(chunkCoord);
}

// Private functions
void WorldGenerationSystem::generateNoise(
    bismuth::Registry& registry,
    bismuth::EntityID  entity,
    glm::ivec2  const& chunkCoord
) {
    auto& chunkMap = registry.getComponentPool<ChunkHeightMapComponent>().getComponent(entity);

    float x = chunkCoord.x ;
    float z = chunkCoord.y ;

    // Perlin chunk size
    const int chunkCoordinateX = static_cast<int>(std::floor(x/VoxelWorlds::PERLIN_SCALE));
    const int chunkCoordinateZ = static_cast<int>(std::floor(z/VoxelWorlds::PERLIN_SCALE));

    const int xOffset = (static_cast<int>(x) % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;
    const int zOffset = (static_cast<int>(z) % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;

    for(int blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
        for(float blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {
            chunkMap.heightMap[blockX * VoxelWorlds::CHUNK_SIZE + blockZ] = std::round(world_generation::generateHeight(
                mSeed, 
                static_cast<int>(blockX + (chunkCoord.x * VoxelWorlds::CHUNK_SIZE)),
                static_cast<int>(blockZ + (chunkCoord.y * VoxelWorlds::CHUNK_SIZE))
            ));
        }
    }
}