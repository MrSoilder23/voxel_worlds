#include "./systems/player_target_system.hpp"

void PlayerTargetSystem::PlayerRaycast(EntityManager& entityManager) {
    auto playerPos = entityManager.GetComponent<PositionComponent>("Player");
    auto playerCam = entityManager.GetComponent<CameraComponent>("Player");
    
    assert(playerPos && playerCam);
        
    EventManager& eventManager = EventManager::GetInstance();
    eventManager.RegisterEvent(InputAction::left_mouse_click, [this, entityManagerPtr = &entityManager, playerPos, playerCam](float _){
        glm::vec3 blockWorldCoords = this->GetBlock(*entityManagerPtr, *playerPos, *playerCam, 0.0001f);
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::string timestamp = std::format("{:%Y-%m-%d %H:%M:%S}", now);

        BlockEventComponent blockEvent;
        blockEvent.mPosition = blockWorldCoords;
        blockEvent.mTimestamp = now;

        entityManagerPtr->CreateEntity(timestamp);
        entityManagerPtr->AddComponent<BlockEventComponent>(timestamp, blockEvent);
        entityManagerPtr->AddComponent<BlockBreakEventComponent>(timestamp);
    });

    eventManager.RegisterEvent(InputAction::right_mouse_click, [this, entityManagerPtr = &entityManager, playerPos, playerCam](float _){
        glm::vec3 blockWorldCoords = this->GetBlock(*entityManagerPtr, *playerPos, *playerCam, -0.0001f);
        auto playerInventory = entityManagerPtr->GetComponent<InventoryComponent>("Player");
        BlockTypes currentBlock = playerInventory->mInventory[playerInventory->mCurrentSlot].mItem;

        auto now = std::chrono::system_clock::now();
        std::string timestamp = std::format("{:%Y-%m-%d %H:%M:%S}", now);

        BlockEventComponent blockEvent;
        blockEvent.mPosition = blockWorldCoords;
        blockEvent.mTimestamp = now;

        BlockPlaceEventComponent blockPlaceEvent;
        blockPlaceEvent.mBlockPlaced = currentBlock;

        entityManagerPtr->CreateEntity(timestamp);
        entityManagerPtr->AddComponent<BlockEventComponent>(timestamp, blockEvent);
        entityManagerPtr->AddComponent<BlockPlaceEventComponent>(timestamp, blockPlaceEvent);
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

    std::array<BoundingBoxCollectionComponent*, 27> chunksBoundings;
    int i = 0;
    for(int chunkX = -1; chunkX <= 1; chunkX++) {
        for(int chunkY = -1; chunkY <= 1; chunkY++) {
            for(int chunkZ = -1; chunkZ <= 1; chunkZ++) {
                glm::ivec3 chunkName = {playerX+chunkX, playerY+chunkY, playerZ+chunkZ};
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
            float distance = physics::LineIntersectsAABB(ray, box);
            
            if(distance != -1 && distance < collisionDistance) {
                collisionDistance = distance;
                globalCoords = ray.mPosition + ray.mDirection * (distance+epsilon);
            }
        }
    }
    
    return (glm::length(globalCoords) != 0) ? globalCoords : playerPos.mPosition;
}