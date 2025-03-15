#include "./systems/chunk_bounding_creation_system.hpp"

void ChunkBoundingCreationSystem::CreateChunkBoundingBoxes(int x, int y, int z) {
    char chunkName[32];
    snprintf(chunkName, sizeof(chunkName), "%d:%d:%d", x, y, z);

    auto chunkData = mEntityManager->GetComponent<ChunkStorageComponent>(chunkName);
    BoundingBoxCollectionComponent bBoxCollection;

    if(!chunkData || !chunkData->mWasGenerated) {
        return;
    }

    auto chunkBounding = mEntityManager->GetComponent<BoundingBoxCollectionComponent>(chunkName);
    if(!chunkBounding) { return; }

    auto chunkModel = mEntityManager->GetComponent<ChunkModelComponent>(chunkName);
    
    if(chunkModel->mModel.indexBufferData.size() == 0) {
        *chunkBounding = std::move(bBoxCollection);
        return;
    }

    bBoxCollection.boundingBoxes.resize(TOTAL_CHUNK_SIZE);
    bBoxCollection.group = Group::terrain;

    for(int blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
        for(int blockY = 0; blockY < VoxelWorlds::CHUNK_SIZE; blockY++) {
            for(int blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {
                auto block = ChunkStorage::GetBlock(*chunkData, blockX,blockY,blockZ);

                if(block == BlockTypes::air) {
                    continue;
                }
                const size_t coords = blockX + blockY * VoxelWorlds::CHUNK_SIZE + blockZ * VoxelWorlds::CHUNK_SIZE * VoxelWorlds::CHUNK_SIZE; 
                assert(coords < bBoxCollection.boundingBoxes.size());

                bBoxCollection.boundingBoxes[coords].mWorldMin = glm::vec3(-0.5f+blockX+VoxelWorlds::CHUNK_SIZE*x,
                                                                      -0.5f+blockY+VoxelWorlds::CHUNK_SIZE*y,
                                                                      -0.5f+blockZ+VoxelWorlds::CHUNK_SIZE*z);

                bBoxCollection.boundingBoxes[coords].mWorldMax = glm::vec3( 0.5f+blockX+VoxelWorlds::CHUNK_SIZE*x, 
                                                                       0.5f+blockY+VoxelWorlds::CHUNK_SIZE*y, 
                                                                       0.5f+blockZ+VoxelWorlds::CHUNK_SIZE*z);
            }
        }
    }

    *chunkBounding = std::move(bBoxCollection);
}

void ChunkBoundingCreationSystem::SetEntityManager(EntityManager& entityManager) {
    mEntityManager = &entityManager;
}