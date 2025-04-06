#include "./systems/bounding_box_system.hpp"

void BoundingBoxSystem::GenerateBoundingBox(EntityManager& entityManager) {
    auto boundingBoxes = entityManager.GetComponentArray<BoundingBoxComponent>();
    auto positions = entityManager.GetComponentArray<PositionComponent>();

    for(size_t entityID = 0; entityID < boundingBoxes.size(); entityID++) {

        if(entityID >= boundingBoxes.size() || entityID >= positions.size()) {
            continue;
        }

        auto boundingBox = boundingBoxes[entityID];
        auto position = positions[entityID];

        if(!boundingBox || !position) {
            continue;
        }
        
        glm::vec3 worldMax = boundingBox->mLocalMax + position->mPosition;
        glm::vec3 worldMin = boundingBox->mLocalMin + position->mPosition;

        if(boundingBox->mWorldMax != worldMax || boundingBox->mWorldMin != worldMin) {
            boundingBox->mWorldMax = worldMax;
            boundingBox->mWorldMin = worldMin;

            boundingBox->mModel = std::move(physics::CreateBoundingModel(*boundingBox));
        }
    }
}

void BoundingBoxSystem::GenerateBoundingBoxSingle(EntityManager& entityManager, std::string entityName) {
    auto boundingBox = entityManager.GetComponent<BoundingBoxComponent>(entityName);
    if(!boundingBox) {
        return;
    }

    auto position = entityManager.GetComponent<PositionComponent>(entityName);
    if(!position) {
        return;
    }

    boundingBox->mWorldMax = boundingBox->mLocalMax + position->mPosition;
    boundingBox->mWorldMin = boundingBox->mLocalMin + position->mPosition;
    
    boundingBox->mModel = std::move(physics::CreateBoundingModel(*boundingBox));
    
}