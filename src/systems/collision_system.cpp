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

    // std::cout << "pos at: " << playerPosition->mPosition.x << ":" << playerPosition->mPosition.y << ":" << playerPosition->mPosition.z << std::endl;
    // std::cout << "collides at: " << playerBounding->mLocalMin.x << ":" << playerBounding->mLocalMin.y << ":" << playerBounding->mLocalMin.z << std::endl;
    // std::cout << "collides a2: " << tempBounding.mMax.x << ":" << tempBounding.mMax.y << ":" << tempBounding.mMax.z << std::endl;
    // std::cout << "collides with: " << playerX << ":" << playerY << ":" << playerZ << std::endl;
    
    char chunkName[32];
    snprintf(chunkName, sizeof(chunkName), "%d:%d:%d", playerX, playerY, playerZ);
    auto chunkBounding = entityManager.GetComponent<BoundingBoxCollectionComponent>(chunkName);
    if(!chunkBounding) {
        return;
    }
    
    glm::vec3 normals = glm::vec3(0.0f);
    glm::vec3 correction(0.0f);

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

    // std::cout << playerPhysics->mVelocity.x << ":" << playerPhysics->mVelocity.y << ":" << playerPhysics->mVelocity.z << std::endl;
    // std::cout << normals.x << ":" << normals.y << ":" << normals.z << std::endl;
    if(collisionTime < 1.0f) {    
        float remainingTime = 1.0f - collisionTime;

        // Remove velocity component along the collision normal
        glm::vec3 velocityAlongNormal = glm::dot(playerPhysics->mVelocity, normals) * normals;
        playerPhysics->mVelocity -= velocityAlongNormal;
    
        playerPhysics->mVelocity *= remainingTime;

        // float dotProd = glm::dot(playerPhysics->mVelocity, normals);    
        // playerPhysics->mVelocity -= dotProd*normals;

    }  
}