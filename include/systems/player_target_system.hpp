#pragma once
// C++ standard libraries
#include <format>
#include <chrono>

// Third_party libraries
#include <SDL2/SDL.h>

// Own libraries
#include "./core/entity_manager.hpp"
#include "./utility/physics.hpp"
#include "./line.hpp"
#include "./components/bounding_box_component.hpp"
#include "./components/chunk_storage_component.hpp"
#include "./components/bounding_box_collection_component.hpp"
#include "./components/position_component.hpp"
#include "./components/inventory_component.hpp"
#include "./components/camera_component.hpp"
#include "./systems/world_generation_system.hpp"
#include "./core/event_manager.hpp"
#include "./components/block_break_event_component.hpp"
#include "./components/block_place_event_component.hpp"
#include "./components/block_event_component.hpp"

class PlayerTargetSystem {
    public:
        void PlayerRaycast(EntityManager& entityManager);
    private:
        glm::vec3 GetBlock(EntityManager& entityManager, const PositionComponent& playerPos, const CameraComponent& playerCam, float epsilon);
};