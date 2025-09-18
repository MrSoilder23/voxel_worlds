#include "./systems/chunk_unload_system.hpp"

void ChunkUnloadSystem::update(bismuth::Registry& registry) {
    static int RenderDistance = (VoxelWorlds::CHUNK_GENERATION_OFFSET + VoxelWorlds::RENDER_DISTANCE) * VoxelWorlds::CHUNK_SIZE;
    auto chunkView  = registry.getView<ChunkTagComponent, PositionComponent>();
    auto playerView = registry.getView<PlayerTagComponent, PositionComponent>();

    glm::vec3 playerPos(0.0f);
    for (auto [entity, player, pos] : playerView) {
        playerPos = pos.position;
        break;
    }

    std::vector<bismuth::EntityID> entitiesToRemove;
    for(auto [entity, chunk, position] : chunkView) {
        float distance = glm::distance(
            glm::vec2(position.position.x, position.position.z),
            glm::vec2(playerPos.x, playerPos.z)
        );
        
        if (distance > RenderDistance) {
            entitiesToRemove.push_back(entity);
        }
    }

    for (bismuth::EntityID entity : entitiesToRemove) {
        registry.removeEntity(entity);
    }
}