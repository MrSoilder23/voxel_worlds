#include "./systems/block_event_system.hpp"

void BlockEventSystem::update(bismuth::Registry& registry) {
    auto chunkView = registry.getView<ChunkTagComponent, PositionComponent>();
    std::unordered_map<glm::ivec3, bismuth::EntityID, IVec3Hash> chunkMap;
    
    for (auto [entity, chunkTag, position] : chunkView) {
        glm::ivec3 chunkCoords = position.position/VoxelWorlds::CHUNK_SIZE;
        chunkMap[chunkCoords] = entity;
    }
    
    auto eventView = registry.getView<BlockEventComponent>();
    
    for (auto [entity, blockEvent] : eventView) {
        int chunkX = static_cast<int>(std::floor(std::round(blockEvent.position.x)/VoxelWorlds::CHUNK_SIZE));
        int chunkY = static_cast<int>(std::floor(std::round(blockEvent.position.y)/VoxelWorlds::CHUNK_SIZE));
        int chunkZ = static_cast<int>(std::floor(std::round(blockEvent.position.z)/VoxelWorlds::CHUNK_SIZE));
        
        glm::ivec3 chunkName = {chunkX, chunkY, chunkZ};
        
        auto chunkIt = chunkMap.find(chunkName);
        if (chunkIt == chunkMap.end()) {
            continue;
        }
        
        bismuth::EntityID chunkEntity = chunkIt->second;
        
        auto& chunkStorage = registry.getComponentPool<ChunkStorageComponent>().getComponent(chunkEntity);
        auto& chunkState = registry.getComponentPool<ChunkStateComponent>().getComponent(chunkEntity);
        
        glm::vec3 chunkWorldPos = chunkName * static_cast<int>(VoxelWorlds::CHUNK_SIZE);
        glm::vec3 localBlockCoordinates = blockEvent.position - chunkWorldPos;
        
        int localBlockX = static_cast<int>(std::round(localBlockCoordinates.x));
        int localBlockY = static_cast<int>(std::round(localBlockCoordinates.y));
        int localBlockZ = static_cast<int>(std::round(localBlockCoordinates.z));
        
        std::array<ChunkStateComponent*, 3> neighborStates = {nullptr, nullptr, nullptr};
        std::array<glm::ivec3, 3> neighborOffsets = {
            glm::ivec3(-1, 0, 0),
            glm::ivec3(0, -1, 0),
            glm::ivec3(0, 0, -1) 
        };
        
        if (localBlockX == 0) {
            glm::ivec3 neighborName = chunkName + neighborOffsets[0];
            if (auto it = chunkMap.find(neighborName); it != chunkMap.end())
                neighborStates[0] = &registry.getComponentPool<ChunkStateComponent>().getComponent(it->second);
        } else if (localBlockX == VoxelWorlds::CHUNK_SIZE-1) {
            glm::ivec3 neighborName = chunkName - neighborOffsets[0];
            if (auto it = chunkMap.find(neighborName); it != chunkMap.end())
                neighborStates[0] = &registry.getComponentPool<ChunkStateComponent>().getComponent(it->second);
        }

        if (localBlockY == 0) {
            glm::ivec3 neighborName = chunkName + neighborOffsets[1];
            if (auto it = chunkMap.find(neighborName); it != chunkMap.end())
                neighborStates[1] = &registry.getComponentPool<ChunkStateComponent>().getComponent(it->second);
        } else if (localBlockY == VoxelWorlds::CHUNK_SIZE-1) {
            glm::ivec3 neighborName = chunkName - neighborOffsets[1];
            if (auto it = chunkMap.find(neighborName); it != chunkMap.end())
                neighborStates[1] = &registry.getComponentPool<ChunkStateComponent>().getComponent(it->second);
        }

        if (localBlockZ == 0) {
            glm::ivec3 neighborName = chunkName + neighborOffsets[2];
            if (auto it = chunkMap.find(neighborName); it != chunkMap.end())
                neighborStates[2] = &registry.getComponentPool<ChunkStateComponent>().getComponent(it->second);
        } else if (localBlockZ == VoxelWorlds::CHUNK_SIZE-1) {
            glm::ivec3 neighborName = chunkName - neighborOffsets[2];
            if (auto it = chunkMap.find(neighborName); it != chunkMap.end())
                neighborStates[2] = &registry.getComponentPool<ChunkStateComponent>().getComponent(it->second);
        }
        
        chunkState.progress = ChunkProgress::pending;
        
        for (auto* neighborState : neighborStates) {
            if (neighborState) {
                neighborState->progress = ChunkProgress::pending;
            }
        }
        
        // Handle block events
        if (registry.hasComponent<BlockBreakEventComponent>(entity)) {
            auto& block = ChunkStorage::getBlock(chunkStorage, localBlockX, localBlockY, localBlockZ);
            block = BlockTypes::air;
        }
        
        if (registry.hasComponent<BlockPlaceEventComponent>(entity)) {
            auto& placeEvent = registry.getComponentPool<BlockPlaceEventComponent>().getComponent(entity);
            auto& block = ChunkStorage::getBlock(chunkStorage, localBlockX, localBlockY, localBlockZ);
            block = placeEvent.blockPlaced;
        }
        
        registry.removeEntity(entity);
    }
}