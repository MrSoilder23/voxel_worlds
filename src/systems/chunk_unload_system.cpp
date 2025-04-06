#include "./systems/chunk_unload_system.hpp"

void ChunkUnloadSystem::UnloadChunks(EntityManager& entityManager) {
    static int RenderDistance = (VoxelWorlds::CHUNK_GENERATION_OFFSET + VoxelWorlds::RENDER_DISTANCE) * VoxelWorlds::CHUNK_SIZE;
    const auto& playerPos = entityManager.GetComponent<PositionComponent>("Player");

    const auto positions = entityManager.GetComponentArray<PositionComponent>();

    const auto chunkEntities = entityManager.GetChunkEntities();

    const glm::vec2 playerPosition2D = {playerPos->mPosition.x, playerPos->mPosition.z};

    for(const auto& entityPair : chunkEntities) {
        const size_t entityID = entityPair.second;

        if(entityID >= positions.size()) {
            continue;
        }

        const auto& position = positions[entityID];

        if(!position) {
            continue;
        }

        if(glm::distance(position->mPosition.xz(), playerPos->mPosition.xz()) > RenderDistance) {
            entityManager.DeleteEntity(entityPair.first);
        }

    }
}