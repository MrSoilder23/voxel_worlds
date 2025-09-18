#include "core/application.hpp"

// Public
void Application::run() {
    mGame.InitializeProgram("Giera", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mSettings.screenWidth, mSettings.screenHeight);

    mSettings.mGraphicsShaderProgram = shader::CreateGraphicsPipeline("./shaders/vert.glsl", "./shaders/frag.glsl");
    mGraphicsApp->mGraphicsPipeline = mSettings.mGraphicsShaderProgram;

    initialize();
    
    mGame.SetEventCallback([this](float dt) {this->input(dt);});
    mGame.SetUpdateCallback([this](float dt) {this->mainLoop(dt);});
    mGame.SetSystemCallback([this](float dt) {this->system(dt);});
    
    mGame.RunLoop();
}

// Private
// Systems
void Application::input(float deltaTime) {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            mGame.StopLoop();
        }
        if(e.type == SDL_KEYDOWN) {
            if(e.key.keysym.sym == SDLK_ESCAPE) {
                mGame.StopLoop();
            }

            if (e.key.keysym.sym == SDLK_F10) {
                mEventManager.GetEvent(InputAction::toggle_debug3, deltaTime);
            }
            if (e.key.keysym.sym == SDLK_F11) {
                mEventManager.GetEvent(InputAction::toggle_debug, deltaTime);
            }
            if (e.key.keysym.sym == SDLK_F12) {
                mEventManager.GetEvent(InputAction::toggle_debug2, deltaTime);
            }
        }
        if(e.type == SDL_MOUSEMOTION) {
            int mouseX = e.motion.xrel;
            int mouseY = e.motion.yrel;
            
            mEventManager.GetMouseMotionEvent(InputAction::mouse_motion, deltaTime, mouseX, mouseY);
        }
        if(e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                mEventManager.GetEvent(InputAction::left_mouse_click, deltaTime);
            }
            if(e.button.button == SDL_BUTTON_RIGHT) {
                mEventManager.GetEvent(InputAction::right_mouse_click, deltaTime);
            }
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_W]) {
        mEventManager.GetEvent(InputAction::move_forward, deltaTime);
    }
    if(state[SDL_SCANCODE_A]) {
        mEventManager.GetEvent(InputAction::move_left, deltaTime);
    }
    if(state[SDL_SCANCODE_S]) {
        mEventManager.GetEvent(InputAction::move_backwards, deltaTime);
    }
    if(state[SDL_SCANCODE_D]) {
        mEventManager.GetEvent(InputAction::move_right, deltaTime);
    }
    if(state[SDL_SCANCODE_LSHIFT]) {
        mEventManager.GetEvent(InputAction::move_down, deltaTime);
    }
    if(state[SDL_SCANCODE_SPACE]) {
        mEventManager.GetEvent(InputAction::move_up, deltaTime);
    }

    if(state[SDL_SCANCODE_1]) {
        mEventManager.GetEvent(InputAction::hotbar_0, deltaTime);
    }
    if(state[SDL_SCANCODE_2]) {
        mEventManager.GetEvent(InputAction::hotbar_1, deltaTime);
    }
    if(state[SDL_SCANCODE_3]) {
        mEventManager.GetEvent(InputAction::hotbar_2, deltaTime);
    }
    if(state[SDL_SCANCODE_4]) {
        mEventManager.GetEvent(InputAction::hotbar_3, deltaTime);
    }
    if(state[SDL_SCANCODE_5]) {
        mEventManager.GetEvent(InputAction::hotbar_4, deltaTime);
    }
    if(state[SDL_SCANCODE_6]) {
        mEventManager.GetEvent(InputAction::hotbar_5, deltaTime);
    }
    if(state[SDL_SCANCODE_7]) {
        mEventManager.GetEvent(InputAction::hotbar_6, deltaTime);
    }
    if(state[SDL_SCANCODE_8]) {
        mEventManager.GetEvent(InputAction::hotbar_7, deltaTime);
    }
    if(state[SDL_SCANCODE_9]) {
        mEventManager.GetEvent(InputAction::hotbar_8, deltaTime);
    }
    if(state[SDL_SCANCODE_0]) {
        mEventManager.GetEvent(InputAction::hotbar_9, deltaTime);
    }
}
void Application::mainLoop(float deltaTime) {
    fpsCounter(deltaTime);
}
void Application::system(float deltaTime) {
    static PositionUpdateSystem posUpdateSystem;
    static VertexSetupSystem vSetupSystem;
    static BoundingBoxSystem boundingBoxSystem;
    static CollisionSystem collisionSystem;
    static PhysicsSystem physSystem;
    static BlockEventSystem blockEventSystem;
    static ChunkMeshingSystem chunkMeshSystem;
    static ChunkUnloadSystem chunkUnloadSystem;
    static ChunkCreationSystem chunkCreationSystem(mSeed);
    static WorldGenerationSystem worldGenSystem(mSeed);
    static PlayerTargetSystem playerTargetSystem;
    static RendererSystem rendererSystem(mGraphicsApp);

    mPlayerControllerSys.update(mRegistry);
    playerTargetSystem.update(mRegistry);

    chunkCreationSystem.update(mRegistry);
    worldGenSystem.update(mRegistry);
    chunkUnloadSystem.update(mRegistry);

    if(mSettings.physics) {
        collisionSystem.update(mRegistry, deltaTime);
    }

    physSystem.update(mRegistry, deltaTime);
    posUpdateSystem.update(mRegistry);

    blockEventSystem.update(mRegistry);

    chunkMeshSystem.update(mRegistry);
    
    boundingBoxSystem.update(mRegistry);
    vSetupSystem.update(mRegistry);

    rendererSystem.update(mRegistry);
}

// Main Loop
void Application::fpsCounter(float deltaTime) {
    static float smoothedFPS = 0.0f;
    static float alpha = 0.1f;  

    float fps = 1.0f/deltaTime;
    smoothedFPS = alpha * fps + (1.0f - alpha) * smoothedFPS;

    std::string newTitle = "Giera, FPS: " + std::to_string(static_cast<int>(smoothedFPS));
    SDL_SetWindowTitle(mGame.GetWindow(), newTitle.data());
}

// Initialization
void Application::initialize() {
    initializeTextures();
    initializeBlocks();

    initializeWorld();

    initializeBaseEntities();
    initializeKeys();
}
void Application::initializeWorld() {
    static std::random_device rndDevice;
    mSeed = rndDevice();
}
void Application::initializeKeys() {
    mEventManager.RegisterEvent(InputAction::exit, [game = &mGame](float _){game->StopLoop();});
    mEventManager.RegisterEvent(InputAction::toggle_debug, [settings = &mSettings](float _){settings->physics = !settings->physics;});
    mEventManager.RegisterEvent(InputAction::toggle_debug2, [settings = &mSettings](float _){settings->boundingDebug = !settings->boundingDebug;});
    mEventManager.RegisterEvent(InputAction::toggle_debug3, [settings = &mSettings](float _){settings->worldGen = !settings->worldGen;});
}

// void RegisterMovementEvent(EventManager& eventManager, bismuth::Registry& entityManager, InputAction action, glm::vec3 movementDirection) {

//     eventManager.RegisterEvent(action, [&eventManager, &entityManager, movementDirection](float deltaTime) mutable {
//         auto player = entityManager.GetComponent<PlayerControllerComponent>("Player");
//         auto playerCamera = entityManager.GetComponent<CameraComponent>("Player");
//         auto playerVelocity = entityManager.GetComponent<PhysicsComponent>("Player");

//         glm::vec3 forwardVector = glm::normalize(glm::vec3(playerCamera->mViewDirection.x, 0.0f, playerCamera->mViewDirection.z));
//         glm::vec3 rightVector = glm::cross(playerCamera->mViewDirection, playerCamera->mUpVector);
//         rightVector = glm::normalize(rightVector);
        
//         glm::vec3 forwardMovement = forwardVector * player->mSpeed;
//         glm::vec3 sidewaysMovement = rightVector * player->mSpeed;
//         glm::vec3 upMovement = playerCamera->mUpVector * player->mSpeed;

//         glm::vec3 movement = (movementDirection.x * sidewaysMovement) + 
//                              (movementDirection.y * upMovement) + 
//                              (movementDirection.z * forwardMovement);

//         glm::vec3 inputVelocity = movement * deltaTime;

//         if(glm::length(inputVelocity) > 0) {
//             glm::vec3 direction = glm::normalize(inputVelocity);
//             float currentSpeedInDirection = glm::dot(playerVelocity->mVelocity, inputVelocity);

//             float allowableSpeed = player->mSpeed - currentSpeedInDirection;

//             if(allowableSpeed <= 0.0f) {
//                 inputVelocity = glm::vec3(0.0f);
//             } else {
//                 float inputMagnitude = glm::length(inputVelocity);
//                 if(inputMagnitude > allowableSpeed) {
//                     inputVelocity = direction * allowableSpeed;
//                 }
//             }
//         }

//         playerVelocity->mVelocity += inputVelocity;
//     });
// }

void Application::initializeBaseEntities() {
    // Player Entity
    const auto& playerEntityID = mRegistry.createEntity();

    CameraComponent camera;
    camera.fov          = glm::radians(45.0f);
    camera.screenWidth  = mSettings.screenWidth;
    camera.screenHeight = mSettings.screenHeight;
    camera.projectionMatrix = glm::infinitePerspective(45.0f, static_cast<float>(mSettings.screenWidth)/static_cast<float>(mSettings.screenHeight), 0.01f);

    PhysicsComponent physics;
    physics.friction = 2.0f;
    
    mRegistry.emplaceComponent<PlayerTagComponent>(playerEntityID);
    mRegistry.emplaceComponent<PlayerControllerComponent>(playerEntityID, mSettings.speed, mSettings.sensitivity);
    mRegistry.emplaceComponent<PositionComponent>(playerEntityID,    glm::vec3(0.0f, world_generation::GenerateHeight(mSeed, 0,0)+1.2f, 0.0f));
    mRegistry.emplaceComponent<CameraComponent>(playerEntityID,      camera);
    mRegistry.emplaceComponent<BoundingBoxComponent>(playerEntityID, glm::vec3(-0.4, -1.5, -0.4), glm::vec3( 0.4,  0.4,  0.4));
    mRegistry.emplaceComponent<PhysicsComponent>(playerEntityID,     physics);
    mRegistry.emplaceComponent<InventoryComponent>(playerEntityID);
    

    // EventManager& eventManager = EventManager::GetInstance();

    // playerInventory->mInventory[0].mItem = BlockTypes::dirt_block;
    // playerInventory->mInventory[1].mItem = BlockTypes::grass_block;
    // playerInventory->mInventory[2].mItem = BlockTypes::stone_block;
    // playerInventory->mInventory[3].mItem = BlockTypes::dirt_block;
    // playerInventory->mInventory[4].mItem = BlockTypes::sand_block;

    // eventManager.RegisterEvent(InputAction::hotbar_0, [&entityManager](...){
    //     auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
    //     playerInventory->mCurrentSlot = 0;
    // });
    // eventManager.RegisterEvent(InputAction::hotbar_1, [&entityManager](...){
    //     auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
    //     playerInventory->mCurrentSlot = 1;
    // });
    // eventManager.RegisterEvent(InputAction::hotbar_2, [&entityManager](...){
    //     auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
    //     playerInventory->mCurrentSlot = 2;
    // });
    // eventManager.RegisterEvent(InputAction::hotbar_3, [&entityManager](...){
    //     auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
    //     playerInventory->mCurrentSlot = 3;
    // });
    // eventManager.RegisterEvent(InputAction::hotbar_4, [&entityManager](...){
    //     auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
    //     playerInventory->mCurrentSlot = 4;
    // });
    // eventManager.RegisterEvent(InputAction::hotbar_5, [&entityManager](...){
    //     auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
    //     playerInventory->mCurrentSlot = 5;
    // });
    // eventManager.RegisterEvent(InputAction::hotbar_6, [&entityManager](...){
    //     auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
    //     playerInventory->mCurrentSlot = 6;
    // });
    // eventManager.RegisterEvent(InputAction::hotbar_7, [&entityManager](...){
    //     auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
    //     playerInventory->mCurrentSlot = 7;
    // });
    // eventManager.RegisterEvent(InputAction::hotbar_8, [&entityManager](...){
    //     auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
    //     playerInventory->mCurrentSlot = 8;
    // });
    // eventManager.RegisterEvent(InputAction::hotbar_9, [&entityManager](...){
    //     auto playerInventory = entityManager.GetComponent<InventoryComponent>("Player");
    //     playerInventory->mCurrentSlot = 9;
    // });

    // RegisterMovementEvent(eventManager, entityManager, InputAction::move_forward,   glm::vec3(0,0,1));
    // RegisterMovementEvent(eventManager, entityManager, InputAction::move_backwards, glm::vec3(0,0,-1));
    // RegisterMovementEvent(eventManager, entityManager, InputAction::move_left,      glm::vec3(-1,0,0));
    // RegisterMovementEvent(eventManager, entityManager, InputAction::move_right,     glm::vec3(1,0,0));
    // RegisterMovementEvent(eventManager, entityManager, InputAction::move_up,        glm::vec3(0,1,0));
    // RegisterMovementEvent(eventManager, entityManager, InputAction::move_down,      glm::vec3(0,-1,0));

    // eventManager.RegisterMouseMotionEvent(InputAction::mouse_motion, [playerPosition, player, playerCamera](float deltaTime, int mouseX, int mouseY){
    //     glm::quat rotation = playerPosition->mRotation;

    //     float yaw = -glm::radians(mouseX * player->mSensitivity * deltaTime);
    //     float pitch = -glm::radians(mouseY * player->mSensitivity * deltaTime);
    
    //     glm::quat yawRotation = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    //     glm::quat pitchRotation = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    
    //     rotation = yawRotation * rotation;
    //     rotation = rotation * pitchRotation;
    
    //     rotation = glm::normalize(rotation);

    //     utility::RotatePosition(*playerPosition, rotation);
    
    //     playerCamera->mViewDirection = glm::rotate(playerPosition->mRotation, glm::vec3(0.0f, 0.0f, -1.0f));
    // });

}
void Application::initializeTextures() {
    TextureManager& textureManager = TextureManager::getInstance();
    BlockTextureCreator& blockTextureCreator = BlockTextureCreator::getInstance();

    SDL_Surface* chunkAtlas = IMG_Load("./assets/AtlasMap.png");


    if (!chunkAtlas) {
        std::cerr << "Failed to load textures!" << std::endl;
        return;
    }

    textureManager.createNewTexture("ChunkTexture", chunkAtlas);

    blockTextureCreator.createTexture("ChunkTexture", chunkAtlas);
}
void Application::initializeBlocks() {
    BlockRegistry& blockRegistry = BlockRegistry::getInstance();

    // Grass block
    blockRegistry.registerBlock(
        BlockTypes::grass_block,
        blockRegistry.createCubeMesh({
            glm::vec2(1,0), // Side
            glm::vec2(2,0), // Top
            glm::vec2(1,0), // Side
            glm::vec2(1,0), // Side
            glm::vec2(1,0), // Side
            glm::vec2(0,0)  // Bot
        })
    );

    // Dirt Block
    blockRegistry.registerBlock(
        BlockTypes::dirt_block,
        blockRegistry.createCubeMesh({
            glm::vec2(0,0),
            glm::vec2(0,0),
            glm::vec2(0,0),
            glm::vec2(0,0),
            glm::vec2(0,0),
            glm::vec2(0,0)
        })
    );

    // Stone Block
    blockRegistry.registerBlock(
        BlockTypes::stone_block,
        blockRegistry.createCubeMesh({
            glm::vec2(4,0),
            glm::vec2(4,0),
            glm::vec2(4,0),
            glm::vec2(4,0),
            glm::vec2(4,0),
            glm::vec2(4,0)
        })
    );

    // Sand Block
    blockRegistry.registerBlock(
        BlockTypes::sand_block,
        blockRegistry.createCubeMesh({
            glm::vec2(3,0),
            glm::vec2(3,0),
            glm::vec2(3,0),
            glm::vec2(3,0),
            glm::vec2(3,0),
            glm::vec2(3,0)
        })
    );
    
}
