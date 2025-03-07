#include "./systems/physics_system.hpp"

void PhysicsSystem::UpdatePosition(EntityManager& entityManager, std::string entityName, float deltaTime) {
    auto entityPhysics = entityManager.GetComponent<PhysicsComponent>(entityName);

    if(!entityPhysics) {
        return;
    }

    auto entityPosition = entityManager.GetComponent<PositionComponent>(entityName);

    utility::MovePosition(*entityPosition, entityPosition->mPosition + (entityPhysics->mVelocity * deltaTime));
}