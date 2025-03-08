#include "./systems/bounding_box_system.hpp"

void BoundingBoxSystem::GenerateBoundingBox(EntityManager& entityManager) {
    for(const auto& componentPointer : entityManager.GetEntities()) {
        auto boundingBox = entityManager.GetComponent<BoundingBoxComponent>(componentPointer.first);
        if(!boundingBox) {
            continue;
        }

        auto position = entityManager.GetComponent<PositionComponent>(componentPointer.first);
        if(!position) {
            continue;
        }
        
        boundingBox->mWorldMax = boundingBox->mLocalMax + position->mPosition;
        boundingBox->mWorldMin = boundingBox->mLocalMin + position->mPosition;

        boundingBox->mModel = std::move(utility::CreateBoundingModel(*boundingBox));
    
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
    
    boundingBox->mModel = std::move(utility::CreateBoundingModel(*boundingBox));
    
}