#include "./systems/position_update_system.hpp"
#include <iostream>

void PositionUpdateSystem::UpdatePositionTransform(EntityManager& entityManager) {
    for(const auto& entityPointer : entityManager.GetEntities()) {
        auto position = entityManager.GetComponent<PositionComponent>(entityPointer.first);
        
        if(position && position->mDirty) {
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position->mPosition);
            glm::mat4 rotationMatrix = glm::toMat4(position->mRotation);
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), position->mScale);

            position->mTransform = translationMatrix * rotationMatrix * scaleMatrix;

            position->mDirty = false; 
        }
    }
}

void PositionUpdateSystem::UpdatePositionTransformSingle(EntityManager& entityManager, std::string entityName) {
    auto position = entityManager.GetComponent<PositionComponent>(entityName);

    if(position && position->mDirty) {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position->mPosition);
        glm::mat4 rotationMatrix = glm::toMat4(position->mRotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), position->mScale);

        position->mTransform = translationMatrix * rotationMatrix * scaleMatrix;

        position->mDirty = false; 
    }
}