#include "./systems/bounding_box_system.hpp"

void BoundingBoxSystem::update(bismuth::Registry& entityManager) {
    auto& boundingPool = entityManager.getComponentPool<BoundingBoxComponent>();
    auto& positionPool = entityManager.getComponentPool<PositionComponent>();

    auto& boundingDenseIDs  = boundingPool.getDenseEntities();
    auto& positionLocations = positionPool.getDenseEntities();

    for(auto& entityID : boundingDenseIDs) {
        if(entityID >= positionLocations.size()) {
            continue;
        }

        auto& boundingBox = boundingPool.getComponent(entityID);
        auto& position    = positionPool.getComponent(entityID);

        glm::vec3 worldMax = boundingBox.localMax + position.position;
        glm::vec3 worldMin = boundingBox.localMin + position.position;

        if(boundingBox.worldMax != worldMax || boundingBox.worldMin != worldMin) {
            boundingBox.worldMax = worldMax;
            boundingBox.worldMin = worldMin;

            // boundingBox.model = std::move(physics::CreateBoundingModel(boundingBox));
        }
    }
}