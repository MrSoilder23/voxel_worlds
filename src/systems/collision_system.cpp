#include "./systems/collision_system.hpp"

void CollisionSystem::UpdateCollision(EntityManager& entityManager, float deltaTime) {
    auto playerPosition = entityManager.GetComponent<PositionComponent>("Player");
    auto playerBounding = entityManager.GetComponent<BoundingBoxComponent>("Player");
    auto playerPhysics = entityManager.GetComponent<PhysicsComponent>("Player");

    if(!playerBounding) {
        return;
    }

    int playerX = static_cast<int>(std::floor(playerPosition->mPosition.x/VoxelWorlds::CHUNK_SIZE));
    int playerY = static_cast<int>(std::floor(playerPosition->mPosition.y/VoxelWorlds::CHUNK_SIZE));
    int playerZ = static_cast<int>(std::floor(playerPosition->mPosition.z/VoxelWorlds::CHUNK_SIZE));
    
    char chunkName[32];
    snprintf(chunkName, sizeof(chunkName), "%d:%d:%d", playerX, playerY, playerZ);
    auto chunkBounding = entityManager.GetComponent<BoundingBoxCollectionComponent>(chunkName);
    if(!chunkBounding) {
        return;
    }
    
    glm::vec3 normals = glm::vec3(0.0f);
    float remainingTime = 1.0f;

    for(int i = 0; i <= 3; i++) {

        float collisionTime = 1.0f;
        for(const auto& box : chunkBounding->boundingBoxes) {   
            glm::vec3 currentNormal;
            float currentTime = utility::SweptAABB(
                *playerBounding, 
                playerPhysics->mVelocity * deltaTime, 
                box, 
                currentNormal
            );
            
            if (currentTime < collisionTime) {
                collisionTime = currentTime;
                normals = currentNormal;
            }

        }
        
        if(collisionTime < 1.0f) {
            glm::vec3 velocityAlongNormal = glm::dot(playerPhysics->mVelocity, normals) * normals;
            playerPhysics->mVelocity -= velocityAlongNormal;            
        }
    }
}