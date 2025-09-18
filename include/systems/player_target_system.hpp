#pragma once
// C++ standard libraries
#include <format>
#include <vector>
#include <chrono>

// Third_party libraries
#include <SDL2/SDL.h>

// Own libraries
#include "bismuth/registry.hpp"
#include "utility/physics.hpp"
#include "line.hpp"
#include "components/bounding_box_component.hpp"
#include "components/chunk_storage_component.hpp"
#include "components/bounding_box_collection_component.hpp"
#include "components/position_component.hpp"
#include "components/inventory_component.hpp"
#include "components/camera_component.hpp"
#include "components/player_tag.hpp"
#include "components/chunk_tag.hpp"
#include "core/event_manager.hpp"
#include "components/block_break_event_component.hpp"
#include "components/block_place_event_component.hpp"
#include "components/block_event_component.hpp"

class PlayerTargetSystem {
    public:
        void update(bismuth::Registry& registry);
    private:
        glm::vec3 getBlock(    
            bismuth::Registry      & registry, 
            PositionComponent const& playerPos, 
            CameraComponent   const& playerCam, 
            float                    epsilon
        );
};