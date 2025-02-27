#include "./systems/bounding_box_system.hpp"

void BoundingBoxSystem::GenerateBoundingBox(EntityManager& entityManager) {
    for(const auto& componentPointer : entityManager.GetEntities()) {
        auto boundingBox = entityManager.GetComponent<BoundingBoxComponent>(componentPointer.first);
        if(!boundingBox || boundingBox->VAO != 0) {
            return;
        }

        auto position = entityManager.GetComponent<PositionComponent>(componentPointer.first);
        
        if(position && boundingBox->VAO == 0) {
            boundingBox->mMax = boundingBox->mMax + position->mPosition;
            boundingBox->mMin = boundingBox->mMin + position->mPosition;

            boundingBox->mModel = std::move(utility::CreateBoundingModel(*boundingBox));
        }
    
    }
}

void BoundingBoxSystem::GenerateBoundingBoxSingle(EntityManager& entityManager, std::string entityName) {
    auto boundingBox = entityManager.GetComponent<BoundingBoxComponent>(entityName);
    if(!boundingBox || boundingBox->VAO != 0) {
        return;
    }

    auto position = entityManager.GetComponent<PositionComponent>(entityName);

    if(position) {
        boundingBox->mMax = boundingBox->mMax + position->mPosition;
        boundingBox->mMin = boundingBox->mMin + position->mPosition;

        boundingBox->mModel = std::move(utility::CreateBoundingModel(*boundingBox));
    }
}