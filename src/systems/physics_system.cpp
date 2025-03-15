#include "./systems/physics_system.hpp"
#include <iostream>
void PhysicsSystem::UpdatePosition(EntityManager& entityManager, float deltaTime) {
    for(const auto& entityPointer : entityManager.GetEntities()) {
        auto entityPhysics = entityManager.GetComponent<PhysicsComponent>(entityPointer.first);
        if(!entityPhysics) {
            continue;
        }
        
        auto entityPosition = entityManager.GetComponent<PositionComponent>(entityPointer.first);
        if(!entityPosition) {
            continue;
        }

        entityPhysics->mVelocity *= 1.0f - entityPhysics->mFriction * deltaTime;

        if (std::abs(glm::length(entityPhysics->mVelocity)) < 0.001f) {
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

    utility::MovePosition(*entityPosition, entityPosition->mPosition + (entityPhysics->mVelocity * deltaTime));
}