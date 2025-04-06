#include "./systems/block_event_system.hpp"

void BlockEventSystem::UpdateChunks(EntityManager& entityManager){ 
    auto blockBreakEvents = entityManager.GetComponentArray<BlockBreakEventComponent>();
    auto blockPlaceEvents = entityManager.GetComponentArray<BlockPlaceEventComponent>();
    auto blockEvents = entityManager.GetComponentArray<BlockEventComponent>();

    auto chunkStorage = entityManager.GetComponentArray<ChunkStorageComponent>();
    auto chunkStates = entityManager.GetComponentArray<ChunkStateComponent>();

    const auto& chunkEntities = entityManager.GetChunkEntities();
    const auto& entities = entityManager.GetEntities();

    for(const auto& entity : entities) {
        size_t entityID = entity.second;

        if(entityID >= blockEvents.size()) {
            continue;
        }

        auto blockEvent = blockEvents[entityID];
        
        if(!blockEvent) {
            continue;
        }

        // Translating global to local
        int chunkX = static_cast<int>(std::floor(std::round(blockEvent->mPosition.x)/VoxelWorlds::CHUNK_SIZE));
        int chunkY = static_cast<int>(std::floor(std::round(blockEvent->mPosition.y)/VoxelWorlds::CHUNK_SIZE));
        int chunkZ = static_cast<int>(std::floor(std::round(blockEvent->mPosition.z)/VoxelWorlds::CHUNK_SIZE));

        glm::vec3 chunkName = {chunkX, chunkY, chunkZ};

        glm::vec3 chunkWorldPos = chunkName * VoxelWorlds::CHUNK_SIZE;
        glm::vec3 localBlockCoordinates = blockEvent->mPosition - chunkWorldPos;

        int localBlockX = static_cast<int>(std::round(localBlockCoordinates.x));
        int localBlockY = static_cast<int>(std::round(localBlockCoordinates.y));
        int localBlockZ = static_cast<int>(std::round(localBlockCoordinates.z));

        // Updating chunk data
        chunkEntityIDs::const_accessor chunkIDAccessor;
        chunkEntities.find(chunkIDAccessor, chunkName);

        ChunkStateComponent* currentChunk = chunkStates[chunkIDAccessor->second];
        ChunkStateComponent* currentChunkX = nullptr;
        ChunkStateComponent* currentChunkY = nullptr;
        ChunkStateComponent* currentChunkZ = nullptr;

        auto& chunkData = chunkStorage[chunkIDAccessor->second];
        if(!currentChunk) {
            return;
        }

        glm::ivec3 neigbourChunkName = chunkName;
        if(localBlockX == 0) {
            neigbourChunkName.x = neigbourChunkName.x - 1;
            chunkEntities.find(chunkIDAccessor, neigbourChunkName);
        } else if(localBlockX == VoxelWorlds::CHUNK_SIZE-1) {
            neigbourChunkName.x = neigbourChunkName.x + 1;
            chunkEntities.find(chunkIDAccessor, neigbourChunkName);
        }
        currentChunkX = chunkStates[chunkIDAccessor->second];

        neigbourChunkName = chunkName;
        if(localBlockY == 0) {
            neigbourChunkName.y = neigbourChunkName.y - 1;
            chunkEntities.find(chunkIDAccessor, neigbourChunkName);
        } else if(localBlockY == VoxelWorlds::CHUNK_SIZE-1) {
            neigbourChunkName.y = neigbourChunkName.y + 1;
            chunkEntities.find(chunkIDAccessor, neigbourChunkName);
        }
        currentChunkY = chunkStates[chunkIDAccessor->second];

        neigbourChunkName = chunkName;
        if(localBlockZ == 0) {
            neigbourChunkName.z = neigbourChunkName.z - 1;
            chunkEntities.find(chunkIDAccessor, neigbourChunkName);
        } else if(localBlockZ == VoxelWorlds::CHUNK_SIZE-1) {
            neigbourChunkName.z = neigbourChunkName.z + 1;
            chunkEntities.find(chunkIDAccessor, neigbourChunkName);
        }
        currentChunkZ = chunkStates[chunkIDAccessor->second];

        if(currentChunk->mProgress == ChunkProgress::fully_generated) {
            currentChunk->mProgress = ChunkProgress::partially_generated;
        }
        if(currentChunkX && currentChunkX->mProgress == ChunkProgress::fully_generated) {
            currentChunkX->mProgress = ChunkProgress::partially_generated;
        }
        if(currentChunkY && currentChunkY->mProgress == ChunkProgress::fully_generated) {
            currentChunkY->mProgress = ChunkProgress::partially_generated;
        }
        if(currentChunkZ && currentChunkZ->mProgress == ChunkProgress::fully_generated) {
            currentChunkZ->mProgress = ChunkProgress::partially_generated;
        }

        auto blockBreakEvent = blockBreakEvents[entityID];
        auto blockPlaceEvent = blockPlaceEvents[entityID];

        auto& currentBlock = ChunkStorage::GetBlock(*chunkData, localBlockX, localBlockY, localBlockZ);
        
        if(blockBreakEvent) {
            currentBlock = BlockTypes::air;
        }

        if(blockPlaceEvent) {
            currentBlock = blockPlaceEvent->mBlockPlaced;
        }

        entityManager.DeleteEntity(entity.first);
    }
}