#pragma once
// Third_party libraries
#include <SDL2/SDL.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
// Own libraries
#include "./core/entity_manager.hpp"
#include "./components/player_controller_component.hpp"
#include "./components/position_component.hpp"
#include "./components/camera_component.hpp"
#include "./components/physics_component.hpp"
#include "./utility/utility.hpp"

class PlayerControllerSystem {
    public:
        void SetFov(float fov);
        void SetScreenSize(float screenWidth, float screenHeight);
        void SetCamera(EntityManager& entityManager, float near);

        void Update(EntityManager& entityManager, float deltaTime);

    private:
        float mFov;

        float mScreenWidth;
        float mScreenHeight;
};