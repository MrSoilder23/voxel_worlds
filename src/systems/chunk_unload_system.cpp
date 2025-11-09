#include "./systems/chunk_unload_system.hpp"

void ChunkUnloadSystem::update(bismuth::Registry& registry) {
    static int RenderDistance = (VoxelWorlds::CHUNK_GENERATION_OFFSET + VoxelWorlds::RENDER_DISTANCE) * VoxelWorlds::CHUNK_SIZE;
    auto chunkView  = registry.getView<ChunkTagComponent, PositionComponent>();
    auto playerView = registry.getView<PlayerTagComponent, PositionComponent>();

    auto [playerEntity, player, playerPos] = *playerView.begin();

    std::vector<bismuth::EntityID> entitiesToRemove;
    for(auto [entity, chunk, position] : chunkView) {
        glm::vec3 difference = position.position - playerPos.position;
        float distance = glm::length(difference);
        
        if (distance > RenderDistance) {
            entitiesToRemove.push_back(entity);
        }
    }

    for (bismuth::EntityID entity : entitiesToRemove) {
        registry.removeEntity(entity);
    }
}