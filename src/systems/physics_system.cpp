#include "./systems/physics_system.hpp"

void PhysicsSystem::UpdatePosition(EntityManager& entityManager, float deltaTime) {
    auto physics = entityManager.GetComponentArray<PhysicsComponent>();
    auto positions = entityManager.GetComponentArray<PositionComponent>();

    for(const auto& entityPair : entityManager.GetEntities()) {
        const size_t& entityID = entityPair.second;

        auto entityPhysics = physics[entityID];
        auto entityPosition = positions[entityID];

        if(!entityPhysics || !entityPosition) {
            continue;
        }
        
        entityPhysics->mVelocity *= 1.0f - entityPhysics->mFriction * deltaTime;

        if (glm::length(entityPhysics->mVelocity) < 0.001f) {
            entityPhysics->mVelocity = glm::vec3(0.0f);
        }
        
        utility::MovePosition(*entityPosition, entityPosition->mPosition + (entityPhysics->mVelocity * deltaTime));
    }
}

void PhysicsSystem::UpdatePositionSingle(EntityManager& entityManager, std::string entityName, float deltaTime) {
    auto entityPhysics = entityManager.GetComponent<PhysicsComponent>(entityName);
    if(!entityPhysics) {
        return;
    }

    auto entityPosition = entityManager.GetComponent<PositionComponent>(entityName);
    if(!entityPosition) {
        return;
    }

    entityPhysics->mVelocity *= 1.0f - entityPhysics->mFriction * deltaTime;

    if (glm::length(entityPhysics->mVelocity) < 0.001f) {
        entityPhysics->mVelocity = glm::vec3(0.0f);
    }
    
    utility::MovePosition(*entityPosition, entityPosition->mPosition + (entityPhysics->mVelocity * deltaTime));
}