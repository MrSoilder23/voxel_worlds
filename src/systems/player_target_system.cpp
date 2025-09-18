#include "./systems/player_target_system.hpp"

void PlayerTargetSystem::update(bismuth::Registry& registry) {
    auto playerView = registry.getView<PlayerTagComponent, PositionComponent, CameraComponent, InventoryComponent>();

    if (playerView.begin() == playerView.end()) {
        return; // No player found
    }

    auto [playerEntity, playerComp, playerPos, playerCam, playerInventory] = *playerView.begin();
    EventManager& eventManager = EventManager::GetInstance();

    eventManager.RegisterEvent(InputAction::left_mouse_click, [this, &registry, playerPos, playerCam, playerEntity](float _){
        glm::vec3 blockWorldCoords = this->getBlock(registry, playerPos, playerCam, 0.0001f);
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

        bismuth::EntityID entity = registry.createEntity();

        registry.emplaceComponent<BlockEventComponent>(entity, blockWorldCoords, now);
        registry.emplaceComponent<BlockBreakEventComponent>(entity, playerEntity);
    });

    eventManager.RegisterEvent(InputAction::right_mouse_click, [this, &registry, playerPos, playerCam, playerInventory, playerEntity](float _){
        glm::vec3 blockWorldCoords = this->getBlock(registry, playerPos, playerCam, -0.0001f);
        BlockTypes currentBlock = playerInventory.inventory[playerInventory.currentSlot].item;
        auto now = std::chrono::system_clock::now();

        bismuth::EntityID entity = registry.createEntity();

        registry.emplaceComponent<BlockEventComponent>(entity, blockWorldCoords, now);
        registry.emplaceComponent<BlockPlaceEventComponent>(entity, currentBlock, playerEntity);
    });
}

// Private
glm::vec3 PlayerTargetSystem::getBlock(
    bismuth::Registry      & registry, 
    PositionComponent const& playerPos, 
    CameraComponent   const& playerCam, 
    float                    epsilon
) {
    Line ray;
    ray.mPosition = playerPos.position;
    ray.mDirection = glm::normalize(playerCam.viewDirection);

    auto chunkView = registry.getView<ChunkTagComponent, BoundingBoxCollectionComponent, PositionComponent>();

    int playerX = static_cast<int>(std::floor(playerPos.position.x/VoxelWorlds::CHUNK_SIZE));
    int playerY = static_cast<int>(std::floor(playerPos.position.y/VoxelWorlds::CHUNK_SIZE));
    int playerZ = static_cast<int>(std::floor(playerPos.position.z/VoxelWorlds::CHUNK_SIZE));

    std::vector<BoundingBoxCollectionComponent*> chunksBoundings;
    for(auto [entity, chunkTag, boundingCollection, position] : chunkView) {
        int chunkX = static_cast<int>(std::floor(position.position.x/VoxelWorlds::CHUNK_SIZE));
        int chunkY = static_cast<int>(std::floor(position.position.y/VoxelWorlds::CHUNK_SIZE));
        int chunkZ = static_cast<int>(std::floor(position.position.z/VoxelWorlds::CHUNK_SIZE));
        
        // Check if this chunk is in the 3x3x3 area around the player
        if (abs(chunkX - playerX) <= 1 && 
            abs(chunkY - playerY) <= 1 && 
            abs(chunkZ - playerZ) <= 1) {
            chunksBoundings.push_back(&boundingCollection);
        }
    }

    float collisionDistance = std::numeric_limits<float>::infinity();
    glm::vec3 globalCoords = glm::vec3(0);

    for(const auto& chunkBounding : chunksBoundings) {
        for(const auto& box : chunkBounding->boundingBoxes) {   
            float distance = physics::LineIntersectsAABB(ray, box);
            
            if(distance != -1 && distance < collisionDistance) {
                collisionDistance = distance;
                globalCoords = ray.mPosition + ray.mDirection * (distance + epsilon);
            }
        }
    }
    
    return (glm::length(globalCoords) != 0) ? globalCoords : playerPos.position;
}