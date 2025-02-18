#include "./systems/player_controller_system.hpp"

void PlayerControllerSystem::SetFov(float fov) {
    mFov = glm::radians(fov);
}
void PlayerControllerSystem::SetScreenSize(float screenWidth, float screenHeight) {
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;
}
void PlayerControllerSystem::SetCamera(EntityManager& entityManager, float near) {
    auto playerCamera = entityManager.GetComponent<CameraComponent>("Player");

    playerCamera->mProjectionMatrix = glm::infinitePerspective(mFov, mScreenWidth/mScreenHeight, near);
}

void PlayerControllerSystem::Update(EntityManager& entityManager, float deltaTime) {
    auto player = entityManager.GetComponent<PlayerControllerComponent>("Player");
    auto playerPosition = entityManager.GetComponent<PositionComponent>("Player");
    auto playerCamera = entityManager.GetComponent<CameraComponent>("Player");

    glm::vec3 rightVector = glm::cross(playerPosition->mViewDirection, playerCamera->mUpVector);
    rightVector = glm::normalize(rightVector);
    
    glm::vec3 forwardMovement = playerPosition->mViewDirection * player->mSpeed * deltaTime;
    glm::vec3 sidewaysMovement = rightVector * player->mSpeed * deltaTime;
    glm::vec3 upMovement = playerCamera->mUpVector * player->mSpeed * deltaTime;
    
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_W]) {
        playerCamera->mEye += forwardMovement;
    }
    if(state[SDL_SCANCODE_S]) {
        playerCamera->mEye -= forwardMovement;
    }
    if(state[SDL_SCANCODE_A]) {
        playerCamera->mEye -= sidewaysMovement;
    }
    if(state[SDL_SCANCODE_D]) {
        playerCamera->mEye += sidewaysMovement;
    }
    if(state[SDL_SCANCODE_SPACE]) {
        playerCamera->mEye += upMovement;
    }
    if(state[SDL_SCANCODE_LSHIFT]) {
        playerCamera->mEye -= upMovement;
    }

    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_MOUSEMOTION) {
            int mouseX = e.motion.xrel;
            int mouseY = e.motion.yrel;

            float yaw = -glm::radians(mouseX * player->mSensitivity * deltaTime);
            float pitch = -glm::radians(mouseY * player->mSensitivity * deltaTime);
        
            glm::quat yawRotation = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        
            glm::quat pitchRotation = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        
            playerPosition->mOrientation = yawRotation * playerPosition->mOrientation;
            playerPosition->mOrientation = playerPosition->mOrientation * pitchRotation;
        
            playerPosition->mOrientation = glm::normalize(playerPosition->mOrientation);
        
            playerPosition->mViewDirection = glm::rotate(playerPosition->mOrientation, glm::vec3(0.0f, 0.0f, -1.0f));
        }
    }
}