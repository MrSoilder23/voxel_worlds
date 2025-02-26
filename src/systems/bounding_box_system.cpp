#include "./systems/bounding_box_system.hpp"

void BoundingBoxSystem::GenerateBoundingBox(EntityManager& entityManager) {
    for(const auto& componentPointer : entityManager.GetEntities()) {
        auto boundingBox = entityManager.GetComponent<BoundingBoxComponent>(componentPointer.first);
        auto position = entityManager.GetComponent<PositionComponent>(componentPointer.first);

        if(boundingBox && position && boundingBox->VAO == 0) {
            boundingBox->mMax = boundingBox->mMax + position->mPosition;
            boundingBox->mMin = boundingBox->mMin + position->mPosition;

            boundingBox->mModel = std::move(utility::CreateBoundingModel(*boundingBox));
        }
    
    }
}

void BoundingBoxSystem::GenerateBoundingBoxSingle(EntityManager& entityManager, std::string entityName) {
    auto boundingBox = entityManager.GetComponent<BoundingBoxComponent>(entityName);
    auto position = entityManager.GetComponent<PositionComponent>(entityName);

    if(boundingBox && position && boundingBox->VAO == 0) {
        boundingBox->mMax = boundingBox->mMax + position->mPosition;
        boundingBox->mMin = boundingBox->mMin + position->mPosition;

        boundingBox->mModel = std::move(utility::CreateBoundingModel(*boundingBox));
    }
}