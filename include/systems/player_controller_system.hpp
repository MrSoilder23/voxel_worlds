#pragma once
// Third_party libraries
#include <SDL2/SDL.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
// Own libraries
#include "./core/entity_manager.hpp"
#include "./core/event_manager.hpp"
#include "./components/player_controller_component.hpp"
#include "./components/position_component.hpp"
#include "./components/camera_component.hpp"
#include "./components/physics_component.hpp"
#include "./components/inventory_component.hpp"

#include "./utility/physics.hpp"
#include "./utility/utility.hpp"

class PlayerControllerSystem {
    public:
        void SetFov(float fov);
        void SetScreenSize(float screenWidth, float screenHeight);
        void SetCamera(EntityManager& entityManager, float zNear);

        void InitializeMovement(EntityManager& entityManager);
        void Update(EntityManager& entityManager);
    
    private:
        void RegisterMovementEvent(EventManager& eventManager, EntityManager& entityManager, InputAction action, glm::vec3 movementDirection);

    private:
        float mFov;

        float mScreenWidth;
        float mScreenHeight;
};