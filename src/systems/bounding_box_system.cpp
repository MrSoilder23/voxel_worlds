#include "./systems/bounding_box_system.hpp"

void BoundingBoxSystem::GenerateBoundingBox(EntityManager& entityManager) {
    for(const auto& componentPointer : entityManager.GetEntities()) {
        auto boundingBox = entityManager.GetComponent<BoundingBoxComponent>(componentPointer.first);
        auto position = entityManager.GetComponent<PositionComponent>(componentPointer.first);
        auto model = entityManager.GetComponent<ModelComponent>(componentPointer.first);

        if(boundingBox && position && model) {
            boundingBox->mMax = boundingBox->mMax + position->mViewDirection;
            boundingBox->mMin = boundingBox->mMin + position->mViewDirection;

            model->mModel = utility::CreateBoundingModel(*boundingBox);
        }
    
    }
}