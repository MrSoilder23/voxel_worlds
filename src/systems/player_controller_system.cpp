#include "./systems/player_controller_system.hpp"

void PlayerControllerSystem::update(bismuth::Registry& registry) {
    auto& cameraPool   = registry.getComponentPool<CameraComponent>();
    auto& positionPool = registry.getComponentPool<PositionComponent>();
    auto& cameraIDs    = cameraPool.getDenseEntities();

    auto& camera       = cameraPool.getComponent(cameraIDs[0]);
    auto& position     = positionPool.getComponent(cameraIDs[0]);

    camera.viewMatrix = glm::lookAt(position.position, position.position + camera.viewDirection, camera.upVector);

    glm::mat4 viewProj = camera.projectionMatrix * camera.viewMatrix;

    physics::ExtractInfiniteFrustumPlanes(viewProj, camera.frustumPlanes);
}