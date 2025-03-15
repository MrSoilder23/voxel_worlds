#include "./systems/player_target_system.hpp"

void PlayerTargetSystem::PlayerRaycast(EntityManager& entityManager) {
    auto playerPos = entityManager.GetComponent<PositionComponent>("Player");
    auto playerCam = entityManager.GetComponent<CameraComponent>("Player");
    
    assert(playerPos && playerCam);
        
    EventManager& eventManager = EventManager::GetInstance();
    eventManager.RegisterEvent(InputAction::left_mouse_click, [this, entityManagerPtr = &entityManager, playerPos, playerCam](float _){
        glm::vec3 blockWorldCoords = this->GetBlock(*entityManagerPtr, *playerPos, *playerCam, 0.0001f);
        this->DestroyBlock(*entityManagerPtr, blockWorldCoords);
    });

    eventManager.RegisterEvent(InputAction::right_mouse_click, [this, entityManagerPtr = &entityManager, playerPos, playerCam](float _){
        glm::vec3 blockWorldCoords = this->GetBlock(*entityManagerPtr, *playerPos, *playerCam, -0.0001f);
        auto playerInventory = entityManagerPtr->GetComponent<InventoryComponent>("Player");
        BlockTypes currentBlock = playerInventory->mInventory[playerInventory->mCurrentSlot].mItem;
        this->PlaceBlock(*entityManagerPtr, currentBlock, blockWorldCoords);
    });
}

// Private
glm::vec3 PlayerTargetSystem::GetBlock(EntityManager& entityManager, const PositionComponent& playerPos, const CameraComponent& playerCam, float epsilon) {
    Line ray;
    ray.mPosition = playerPos.mPosition;
    ray.mDirection = glm::normalize(playerCam.mViewDirection);

    int playerX = static_cast<int>(std::floor(playerPos.mPosition.x/VoxelWorlds::CHUNK_SIZE));
    int playerY = static_cast<int>(std::floor(playerPos.mPosition.y/VoxelWorlds::CHUNK_SIZE));
    int playerZ = static_cast<int>(std::floor(playerPos.mPosition.z/VoxelWorlds::CHUNK_SIZE));

    char chunkName[32];
    std::array<std::shared_ptr<BoundingBoxCollectionComponent>, 27> chunksBoundings;
    int i = 0;
    for(int chunkX = -1; chunkX <= 1; chunkX++) {
        for(int chunkY = -1; chunkY <= 1; chunkY++) {
            for(int chunkZ = -1; chunkZ <= 1; chunkZ++) {
                snprintf(chunkName, sizeof(chunkName), "%d:%d:%d", playerX+chunkX, playerY+chunkY, playerZ+chunkZ);
                chunksBoundings[i] = entityManager.GetComponent<BoundingBoxCollectionComponent>(chunkName);
                i++;
            }
        }
    }

    float collisionDistance = std::numeric_limits<float>::infinity();
    glm::vec3 globalCoords = glm::vec3(0);

    for(const auto& chunkBounding : chunksBoundings) {
        if(!chunkBounding) {
            continue;
        }

        for(const auto& box : chunkBounding->boundingBoxes) {   
            float distance = utility::LineIntersectsAABB(ray, box);
            
            if(distance != -1 && distance < collisionDistance) {
                collisionDistance = distance;
                globalCoords = ray.mPosition + ray.mDirection * (distance+epsilon);
            }
        }
    }
    
    return (glm::length(globalCoords) != 0) ? globalCoords : playerPos.mPosition;
}
void PlayerTargetSystem::DestroyBlock(EntityManager& entityManager, glm::vec3& globalBlockCoordinates) {
    int chunkX = static_cast<int>(std::floor(std::round(globalBlockCoordinates.x)/VoxelWorlds::CHUNK_SIZE));
    int chunkY = static_cast<int>(std::floor(std::round(globalBlockCoordinates.y)/VoxelWorlds::CHUNK_SIZE));
    int chunkZ = static_cast<int>(std::floor(std::round(globalBlockCoordinates.z)/VoxelWorlds::CHUNK_SIZE));

    char chunkName[32];
    snprintf(chunkName, sizeof(chunkName), "%d:%d:%d", chunkX, chunkY, chunkZ);

    auto chunkData = entityManager.GetComponent<ChunkStorageComponent>(chunkName);
    if(!chunkData) {
        return;
    }

    glm::vec3 chunkWorldPos = glm::vec3(chunkX * VoxelWorlds::CHUNK_SIZE, 
                                        chunkY * VoxelWorlds::CHUNK_SIZE, 
                                        chunkZ * VoxelWorlds::CHUNK_SIZE);

    glm::vec3 localBlockCoordinates = globalBlockCoordinates - chunkWorldPos;

    int localBlockX = static_cast<int>(std::round(localBlockCoordinates.x));
    int localBlockY = static_cast<int>(std::round(localBlockCoordinates.y));
    int localBlockZ = static_cast<int>(std::round(localBlockCoordinates.z));

    ChunkStorage::GetBlock(*chunkData, localBlockX, localBlockY, localBlockZ) = BlockTypes::air;

    WorldGenerationSystem wGen;
    wGen.SetEntityManager(entityManager);
    wGen.GenerateModel(chunkX, chunkY, chunkZ);
}
void PlayerTargetSystem::PlaceBlock(EntityManager& entityManager, BlockTypes block, glm::vec3& globalBlockCoordinates) {
    int chunkX = static_cast<int>(std::floor(std::round(globalBlockCoordinates.x)/VoxelWorlds::CHUNK_SIZE));
    int chunkY = static_cast<int>(std::floor(std::round(globalBlockCoordinates.y)/VoxelWorlds::CHUNK_SIZE));
    int chunkZ = static_cast<int>(std::floor(std::round(globalBlockCoordinates.z)/VoxelWorlds::CHUNK_SIZE));

    char chunkName[32];
    snprintf(chunkName, sizeof(chunkName), "%d:%d:%d", chunkX, chunkY, chunkZ);

    auto chunkData = entityManager.GetComponent<ChunkStorageComponent>(chunkName);
    if(!chunkData) {
        return;
    }

    glm::vec3 chunkWorldPos = glm::vec3(chunkX * VoxelWorlds::CHUNK_SIZE, 
                                        chunkY * VoxelWorlds::CHUNK_SIZE, 
                                        chunkZ * VoxelWorlds::CHUNK_SIZE);

    glm::vec3 localBlockCoordinates = globalBlockCoordinates - chunkWorldPos;

    int localBlockX = static_cast<int>(std::round(localBlockCoordinates.x));
    int localBlockY = static_cast<int>(std::round(localBlockCoordinates.y));
    int localBlockZ = static_cast<int>(std::round(localBlockCoordinates.z));

    auto& currentBlock = ChunkStorage::GetBlock(*chunkData, localBlockX, localBlockY, localBlockZ);

    if(currentBlock == BlockTypes::air) {
        currentBlock = block;
    }

    WorldGenerationSystem wGen;
    wGen.SetEntityManager(entityManager);
    wGen.GenerateModel(chunkX, chunkY, chunkZ);
}