#include "./systems/collision_system.hpp"

void CollisionSystem::update(bismuth::Registry& registry, float deltaTime) {
    auto playerView = registry.getView<PlayerTagComponent, PositionComponent, BoundingBoxComponent, PhysicsComponent>();
    auto chunkView  = registry.getView<ChunkTagComponent, BoundingBoxCollectionComponent, PositionComponent>();

    if (playerView.begin() == playerView.end()) {
        return;
    }

    auto [playerEntity, playerComp, playerPosition, playerBounding, playerPhysics] = *playerView.begin();

    int playerX = static_cast<int>(std::floor(playerPosition.position.x/VoxelWorlds::CHUNK_SIZE));
    int playerY = static_cast<int>(std::floor(playerPosition.position.y/VoxelWorlds::CHUNK_SIZE));
    int playerZ = static_cast<int>(std::floor(playerPosition.position.z/VoxelWorlds::CHUNK_SIZE));
    
    std::vector<BoundingBoxCollectionComponent*> chunksBoundings;
    for (auto [entity, chunkTag, boundingCollection, position] : chunkView) {
            int chunkX = static_cast<int>(std::floor(position.position.x / VoxelWorlds::CHUNK_SIZE));
            int chunkY = static_cast<int>(std::floor(position.position.y / VoxelWorlds::CHUNK_SIZE));
            int chunkZ = static_cast<int>(std::floor(position.position.z / VoxelWorlds::CHUNK_SIZE));
            
            if (abs(chunkX - playerX) <= 1 && 
                abs(chunkY - playerY) <= 1 && 
                abs(chunkZ - playerZ) <= 1) {
                chunksBoundings.push_back(&boundingCollection);
            }
        }

    
    glm::vec3 normals = glm::vec3(0.0f);
    float remainingTime = 1.0f;

    for(int i = 0; i <= 3; i++) {
        float collisionTime = 1.0f;

        for(const auto& chunkBounding : chunksBoundings) {            
            for(const auto& box : chunkBounding->boundingBoxes) {   
                glm::vec3 currentNormal;
                float currentTime = physics::SweptAABB(
                    playerBounding, 
                    playerPhysics.velocity * deltaTime, 
                    box, 
                    currentNormal
                );
                
                if (currentTime < collisionTime) {
                    collisionTime = currentTime;
                    normals = currentNormal;
                }
    
            }
        }
        
        if(collisionTime < 1.0f) {
            glm::vec3 velocityAlongNormal = glm::dot(playerPhysics.velocity, normals) * normals;
            playerPhysics.velocity -= velocityAlongNormal;            
        }
    }
}