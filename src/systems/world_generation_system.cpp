#include "./systems/world_generation_system.hpp"

WorldGenerationSystem::WorldGenerationSystem(unsigned int seed) : mSeed(seed) {}

void WorldGenerationSystem::update(bismuth::Registry& registry) {
    auto playerView = registry.getView<PlayerTagComponent, PositionComponent>();
    auto chunkView  = registry.getView<ChunkTagComponent, PositionComponent>();
    auto heightView = registry.getView<ChunkHeightMapComponent, PositionComponent>();

    if (playerView.begin() == playerView.end()) {
        return;
    }

    mExistingChunks.clear();
    mGeneratedHeightMaps.clear();

    for(auto [entity, chunk, position] : chunkView) {
        glm::ivec3 chunkCoords = glm::floor(position.position / (float)VoxelWorlds::CHUNK_SIZE);
        mExistingChunks.insert(chunkCoords);
    }
    
    for(auto [entity, chunk, position] : heightView) {
        glm::ivec2 chunkCoords = glm::ivec2(position.position.x / VoxelWorlds::CHUNK_SIZE, position.position.z / VoxelWorlds::CHUNK_SIZE);
        mGeneratedHeightMaps.insert(chunkCoords);
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
    static int maxDistance = VoxelWorlds::RENDER_DISTANCE + VoxelWorlds::CHUNK_GENERATION_OFFSET;

    glm::ivec3 playerChunk = glm::floor(playerPosition / float(VoxelWorlds::CHUNK_SIZE));
    
    for(int distance = 0; distance <= maxDistance; distance++) {
        generateRing(registry, playerChunk, distance);
    }
}

void WorldGenerationSystem::generateRing(
    bismuth::Registry& registry,
    glm::ivec3         playerChunk,
    int                currentDistance
) {
    static int maxDistance = VoxelWorlds::RENDER_DISTANCE + VoxelWorlds::CHUNK_GENERATION_OFFSET;

    for(int dx = -maxDistance; dx <= maxDistance; dx++) {
        int adx = dx < 0 ? -dx : dx;
        for(int dy = -maxDistance; dy <= maxDistance; dy++) {
            int ady = dy < 0 ? -dy : dy;
            for(int dz = -maxDistance; dz <= maxDistance; dz++) {
                int adz = dz < 0 ? -dz : dz;
                if (adx + ady + adz != currentDistance) {
                    continue;
                }

                int x = playerChunk.x + dx;
                int y = playerChunk.y + dy;
                int z = playerChunk.z + dz;

                glm::ivec3 chunkCoord(x,y,z);

                if (mExistingChunks.contains(chunkCoord) || 
                    mQueuedChunks.contains(chunkCoord)) {
                    continue;
                }

                std::cout << "ASDASD" << std::endl;

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

    float x = chunkCoord.x;
    float z = chunkCoord.y;

    for(int blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
        for(int blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {
            chunkMap.heightMap[blockX * VoxelWorlds::CHUNK_SIZE + blockZ] = std::round(world_generation::generateHeight(
                mSeed, 
                static_cast<int>(blockX + (x * VoxelWorlds::CHUNK_SIZE)),
                static_cast<int>(blockZ + (z * VoxelWorlds::CHUNK_SIZE))
            ));
        }
    }
}