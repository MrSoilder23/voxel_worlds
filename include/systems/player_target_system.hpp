#pragma once
// Third_party libraries
#include <SDL2/SDL.h>

// Own libraries
#include "./core/entity_manager.hpp"
#include "./utility/utility.hpp"
#include "./line.hpp"
#include "./components/bounding_box_component.hpp"
#include "./components/chunk_storage_component.hpp"
#include "./components/bounding_box_collection_component.hpp"
#include "./components/position_component.hpp"
#include "./components/camera_component.hpp"
#include "./systems/world_generation_system.hpp"

class PlayerTargetSystem {
    public:
        void PlayerRaycast(EntityManager& entityManager);
    private:
        glm::vec3 GetBlock(EntityManager& entityManager, const PositionComponent& playerPos, const CameraComponent& playerCam);
        void DestroyBlock(EntityManager& entityManager, glm::vec3& globalBlockCoordinates);
        void PlaceBlock(EntityManager& entityManager, glm::vec3& globalBlockCoordinates);
};