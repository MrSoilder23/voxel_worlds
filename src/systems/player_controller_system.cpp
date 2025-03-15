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

void PlayerControllerSystem::InitializeMovement(EntityManager& entityManager) {
    auto player = entityManager.GetComponent<PlayerControllerComponent>("Player");
    auto playerPosition = entityManager.GetComponent<PositionComponent>("Player");
    auto playerCamera = entityManager.GetComponent<CameraComponent>("Player");
    auto playerVelocity = entityManager.GetComponent<PhysicsComponent>("Player");

    assert(player || playerPosition || playerCamera || playerVelocity);

    EventManager& eventManager = EventManager::GetInstance();

    RegisterMovementEvent(eventManager, entityManager, InputAction::move_forward,   glm::vec3(0,0,1));
    RegisterMovementEvent(eventManager, entityManager, InputAction::move_backwards, glm::vec3(0,0,-1));
    RegisterMovementEvent(eventManager, entityManager, InputAction::move_left,      glm::vec3(-1,0,0));
    RegisterMovementEvent(eventManager, entityManager, InputAction::move_right,     glm::vec3(1,0,0));
    RegisterMovementEvent(eventManager, entityManager, InputAction::move_up,        glm::vec3(0,1,0));
    RegisterMovementEvent(eventManager, entityManager, InputAction::move_down,      glm::vec3(0,-1,0));

    eventManager.RegisterMouseMotionEvent(InputAction::mouse_motion, [playerPosition, player, playerCamera](float deltaTime, int mouseX, int mouseY){
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
    });
    
}

void PlayerControllerSystem::Update(EntityManager& entityManager) {
    auto player = entityManager.GetComponent<PlayerControllerComponent>("Player");
    auto playerVelocity = entityManager.GetComponent<PhysicsComponent>("Player");

    if (glm::length(playerVelocity->mVelocity) > player->mSpeed) {
        playerVelocity->mVelocity = glm::normalize(playerVelocity->mVelocity) * player->mSpeed;
    }
}

void PlayerControllerSystem::RegisterMovementEvent(EventManager& eventManager, EntityManager& entityManager, InputAction action, glm::vec3 movementDirection) {

    eventManager.RegisterEvent(action, [&eventManager, &entityManager, movementDirection](float deltaTime) mutable {
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

        glm::vec3 movement = (movementDirection.x * sidewaysMovement) + 
                             (movementDirection.y * upMovement) + 
                             (movementDirection.z * forwardMovement);

        playerVelocity->mVelocity += movement * deltaTime;
    });
}