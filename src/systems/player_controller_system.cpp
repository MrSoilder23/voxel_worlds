#include "./systems/player_controller_system.hpp"

void PlayerControllerSystem::SetFov(float fov) {
    mFov = glm::radians(fov);
}
void PlayerControllerSystem::SetScreenSize(float screenWidth, float screenHeight) {
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;
}
void PlayerControllerSystem::SetCamera(EntityManager& entityManager, float zNear) {
    auto playerCamera = entityManager.GetComponent<CameraComponent>("Player");

    playerCamera->mProjectionMatrix = glm::infinitePerspective(mFov, mScreenWidth/mScreenHeight, zNear);
}

void PlayerControllerSystem::InitializeMovement(EntityManager& entityManager) {
    auto player = entityManager.GetComponent<PlayerControllerComponent>("Player");
    auto playerPosition = entityManager.GetComponent<PositionComponent>("Player");
    auto playerCamera = entityManager.GetComponent<CameraComponent>("Player");
    auto playerVelocity = entityManager.GetComponent<PhysicsComponent>("Player");
    auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");

    assert(player && playerPosition && playerCamera && playerVelocity && playerInventory);
    EventManager& eventManager = EventManager::GetInstance();

    playerInventory->mInventory[0].mItem = BlockTypes::dirt_block;
    playerInventory->mInventory[1].mItem = BlockTypes::grass_block;
    playerInventory->mInventory[2].mItem = BlockTypes::stone_block;
    playerInventory->mInventory[3].mItem = BlockTypes::dirt_block;
    playerInventory->mInventory[4].mItem = BlockTypes::sand_block;

    eventManager.RegisterEvent(InputAction::hotbar_0, [&entityManager](...){
        auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
        playerInventory->mCurrentSlot = 0;
    });
    eventManager.RegisterEvent(InputAction::hotbar_1, [&entityManager](...){
        auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
        playerInventory->mCurrentSlot = 1;
    });
    eventManager.RegisterEvent(InputAction::hotbar_2, [&entityManager](...){
        auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
        playerInventory->mCurrentSlot = 2;
    });
    eventManager.RegisterEvent(InputAction::hotbar_3, [&entityManager](...){
        auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
        playerInventory->mCurrentSlot = 3;
    });
    eventManager.RegisterEvent(InputAction::hotbar_4, [&entityManager](...){
        auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
        playerInventory->mCurrentSlot = 4;
    });
    eventManager.RegisterEvent(InputAction::hotbar_5, [&entityManager](...){
        auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
        playerInventory->mCurrentSlot = 5;
    });
    eventManager.RegisterEvent(InputAction::hotbar_6, [&entityManager](...){
        auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
        playerInventory->mCurrentSlot = 6;
    });
    eventManager.RegisterEvent(InputAction::hotbar_7, [&entityManager](...){
        auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
        playerInventory->mCurrentSlot = 7;
    });
    eventManager.RegisterEvent(InputAction::hotbar_8, [&entityManager](...){
        auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
        playerInventory->mCurrentSlot = 8;
    });
    eventManager.RegisterEvent(InputAction::hotbar_9, [&entityManager](...){
        auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
        playerInventory->mCurrentSlot = 9;
    });

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
    auto playerCamera = entityManager.GetComponent<CameraComponent>("Player");
    auto playerPosition = entityManager.GetComponent<PositionComponent>("Player");

    playerCamera->mViewMatrix = glm::lookAt(playerPosition->mPosition, playerPosition->mPosition + playerCamera->mViewDirection, playerCamera->mUpVector);

    glm::mat4 viewProj = playerCamera->mProjectionMatrix * playerCamera->mViewMatrix;

    physics::ExtractInfiniteFrustumPlanes(viewProj, playerCamera->frustumPlanes);
}

void PlayerControllerSystem::RegisterMovementEvent(EventManager& eventManager, EntityManager& entityManager, InputAction action, glm::vec3 movementDirection) {

    eventManager.RegisterEvent(action, [&eventManager, &entityManager, movementDirection](float deltaTime) mutable {
        auto player = entityManager.GetComponent<PlayerControllerComponent>("Player");
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

        glm::vec3 inputVelocity = movement * deltaTime;

        if(glm::length(inputVelocity) > 0) {
            glm::vec3 direction = glm::normalize(inputVelocity);
            float currentSpeedInDirection = glm::dot(playerVelocity->mVelocity, inputVelocity);

            float allowableSpeed = player->mSpeed - currentSpeedInDirection;

            if(allowableSpeed <= 0.0f) {
                inputVelocity = glm::vec3(0.0f);
            } else {
                float inputMagnitude = glm::length(inputVelocity);
                if(inputMagnitude > allowableSpeed) {
                    inputVelocity = direction * allowableSpeed;
                }
            }
        }

        playerVelocity->mVelocity += inputVelocity;
    });
}