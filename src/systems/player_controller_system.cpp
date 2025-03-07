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
    auto playerVelocity = entityManager.GetComponent<PhysicsComponent>("Player");

    glm::vec3 forwardVector = glm::normalize(glm::vec3(playerCamera->mViewDirection.x, 0.0f, playerCamera->mViewDirection.z));
    glm::vec3 rightVector = glm::cross(playerCamera->mViewDirection, playerCamera->mUpVector);
    rightVector = glm::normalize(rightVector);
    
    glm::vec3 forwardMovement = forwardVector * player->mSpeed;
    glm::vec3 sidewaysMovement = rightVector * player->mSpeed;
    glm::vec3 upMovement = playerCamera->mUpVector * player->mSpeed;
    
    glm::vec3 velocity = glm::vec3(0.0f);

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_W]) {
        velocity += forwardMovement; 
    }
    if(state[SDL_SCANCODE_S]) {
        velocity -= forwardMovement;
    }
    if(state[SDL_SCANCODE_A]) {
        velocity -= sidewaysMovement;
    }
    if(state[SDL_SCANCODE_D]) {
        velocity += sidewaysMovement;
    }
    if(state[SDL_SCANCODE_SPACE]) {
        velocity += upMovement;
    }
    if(state[SDL_SCANCODE_LSHIFT]) {
        velocity -= upMovement;
    }
    
    playerVelocity->mVelocity += velocity * deltaTime;
    if(glm::length(playerVelocity->mVelocity) > player->mSpeed) {
        playerVelocity->mVelocity = glm::normalize(playerVelocity->mVelocity) * player->mSpeed;
    }
    
    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_MOUSEMOTION) {
            int mouseX = e.motion.xrel;
            int mouseY = e.motion.yrel;

            glm::quat rotation = playerPosition->mRotation;

            float yaw = -glm::radians(mouseX * player->mSensitivity * deltaTime);
            float pitch = -glm::radians(mouseY * player->mSensitivity * deltaTime);
        
            glm::quat yawRotation = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::quat pitchRotation = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        
            rotation = yawRotation * rotation;
            rotation = rotation * pitchRotation;
        
            rotation = glm::normalize(rotation);

            utility::RotatePosition(*playerPosition, rotation);
        
            playerCamera->mViewDirection = glm::rotate(playerPosition->mRotation, glm::vec3(0.0f, 0.0f, -1.0f));
        }
    }
}