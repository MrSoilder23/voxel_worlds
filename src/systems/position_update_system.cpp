#include "./systems/position_update_system.hpp"

void PositionUpdateSystem::update(bismuth::Registry& entityManager) {
    auto& positionPool = entityManager.getComponentPool<PositionComponent>();
    
    auto itEnd = positionPool.componentEnd();
    for(auto position = positionPool.componentBegin(); position != itEnd; ++position) {
        if(position->dirty) {
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position->position);
            glm::mat4 rotationMatrix = glm::toMat4(position->rotation);
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), position->scale);

            position->transform = translationMatrix * rotationMatrix * scaleMatrix;

            position->dirty = false; 
        }
    }
}