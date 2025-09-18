#include "./systems/physics_system.hpp"

void PhysicsSystem::update(bismuth::Registry& registry, float deltaTime) {
    auto physicsView = registry.getView<PhysicsComponent, PositionComponent>();

    for(auto [entity, physics, position] : physicsView) {
        physics.velocity *= 1.0f - physics.friction * deltaTime;

        if (glm::length(physics.velocity) < 0.001f) {
            physics.velocity = glm::vec3(0.0f);
        }
        
        utility::MovePosition(position, position.position + (physics.velocity * deltaTime));
    }
}