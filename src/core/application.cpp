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
    static StoneGenerationSystem stoneGenerationSystem(mSeed);
    static WorldGenerationSystem worldGenSystem(mSeed);
    static RendererSystem rendererSystem(mGraphicsApp);

    mPlayerControllerSys.update(mRegistry);

    if(mSettings.worldGen) {
        worldGenSystem.update(mRegistry);
        stoneGenerationSystem.update(mRegistry);
        chunkUnloadSystem.update(mRegistry);
    }

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
    // static std::random_device rndDevice;
    // mSeed = rndDevice();
}
void Application::initializeKeys() {
    mEventManager.RegisterEvent(InputAction::exit, [game = &mGame](float _){game->StopLoop();});
    mEventManager.RegisterEvent(InputAction::toggle_debug, [settings = &mSettings](float _){settings->physics = !settings->physics;});
    mEventManager.RegisterEvent(InputAction::toggle_debug2, [settings = &mSettings](float _){settings->boundingDebug = !settings->boundingDebug;});
    mEventManager.RegisterEvent(InputAction::toggle_debug3, [settings = &mSettings](float _){settings->worldGen = !settings->worldGen;});
}

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

    InventoryComponent inventory;
    inventory.inventory[0].item = BlockTypes::dirt_block;
    inventory.inventory[1].item = BlockTypes::grass_block;
    inventory.inventory[2].item = BlockTypes::stone_block;
    inventory.inventory[3].item = BlockTypes::sand_block;
    inventory.inventory[4].item = BlockTypes::dirt_block;
    inventory.inventory[5].item = BlockTypes::dirt_block;
    inventory.inventory[6].item = BlockTypes::dirt_block;
    inventory.inventory[7].item = BlockTypes::dirt_block;
    inventory.inventory[8].item = BlockTypes::dirt_block;
    inventory.inventory[9].item = BlockTypes::dirt_block;
    
    mRegistry.emplaceComponent<PlayerTagComponent>(playerEntityID);
    mRegistry.emplaceComponent<PlayerControllerComponent>(playerEntityID, mSettings.speed, mSettings.sensitivity);
    mRegistry.emplaceComponent<PositionComponent>(playerEntityID,    glm::vec3(0.0f, world_generation::generateHeight(mSeed, 0,0)+1.2f, 0.0f));
    mRegistry.emplaceComponent<CameraComponent>(playerEntityID,      camera);
    mRegistry.emplaceComponent<BoundingBoxComponent>(playerEntityID, glm::vec3(-0.4, -1.5, -0.4), glm::vec3( 0.4,  0.4,  0.4));
    mRegistry.emplaceComponent<PhysicsComponent>(playerEntityID,     physics);
    mRegistry.emplaceComponent<InventoryComponent>(playerEntityID,   inventory);
    

    EventManager& eventManager = EventManager::GetInstance();

    for (int i = 0; i <= 9; i++) {
        eventManager.RegisterEvent(static_cast<InputAction>(static_cast<int>(InputAction::hotbar_0) + i), 
            [this, i, playerEntityID](float _) {
                if (mRegistry.hasComponent<InventoryComponent>(playerEntityID)) {
                    auto& inventory = mRegistry.getComponentPool<InventoryComponent>().getComponent(playerEntityID);
                    inventory.currentSlot = i;
                }
            });
    }

    auto registerMovementEvent = [this, playerEntityID](InputAction action, glm::vec3 direction) {
        mEventManager.RegisterEvent(action, [this, playerEntityID, direction](float deltaTime) {
            // Access components through registry
            auto& player = mRegistry.getComponentPool<PlayerControllerComponent>().getComponent(playerEntityID);
            auto& camera = mRegistry.getComponentPool<CameraComponent>().getComponent(playerEntityID);
            auto& physics = mRegistry.getComponentPool<PhysicsComponent>().getComponent(playerEntityID);

            glm::vec3 forwardVector = glm::normalize(glm::vec3(camera.viewDirection.x, 0.0f, camera.viewDirection.z));
            glm::vec3 rightVector = glm::cross(camera.viewDirection, camera.upVector);
            rightVector = glm::normalize(rightVector);
            
            glm::vec3 forwardMovement = forwardVector * player.speed;
            glm::vec3 sidewaysMovement = rightVector * player.speed;
            glm::vec3 upMovement = camera.upVector * player.speed;

            glm::vec3 movement = (direction.x * sidewaysMovement) +
                                 (direction.y * upMovement) +
                                 (direction.z * forwardMovement);

            glm::vec3 inputVelocity = movement * deltaTime;

            if(glm::length(inputVelocity) > 0) {
                glm::vec3 direction = glm::normalize(inputVelocity);
                float currentSpeedInDirection = glm::dot(physics.velocity, inputVelocity);

                float allowableSpeed = player.speed - currentSpeedInDirection;

                if(allowableSpeed <= 0.0f) {
                    inputVelocity = glm::vec3(0.0f);
                } else {
                    float inputMagnitude = glm::length(inputVelocity);
                    if(inputMagnitude > allowableSpeed) {
                        inputVelocity = direction * allowableSpeed;
                    }
                }
            }

            physics.velocity += inputVelocity;
        });
    };

    registerMovementEvent(InputAction::move_forward,   glm::vec3(0,0,1));
    registerMovementEvent(InputAction::move_backwards, glm::vec3(0,0,-1));
    registerMovementEvent(InputAction::move_left,      glm::vec3(-1,0,0));
    registerMovementEvent(InputAction::move_right,     glm::vec3(1,0,0));
    registerMovementEvent(InputAction::move_up,        glm::vec3(0,1,0));
    registerMovementEvent(InputAction::move_down,      glm::vec3(0,-1,0));

    eventManager.RegisterMouseMotionEvent(InputAction::mouse_motion, [this](float deltaTime, int mouseX, int mouseY){
        auto playerView = mRegistry.getView<PlayerTagComponent, PlayerControllerComponent, PositionComponent, CameraComponent>();

        if (playerView.begin() == playerView.end()) {
            return;
        }

        auto [entity, playerTag, player, position, camera] = *playerView.begin();

        glm::quat rotation = position.rotation;

        float yaw = -glm::radians(mouseX * player.sensitivity);
        float pitch = -glm::radians(mouseY * player.sensitivity);
    
        glm::quat yawRotation = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat pitchRotation = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    
        rotation = yawRotation * rotation;
        rotation = rotation * pitchRotation;
    
        rotation = glm::normalize(rotation);

        utility::RotatePosition(position, rotation);
    
        camera.viewDirection = glm::rotate(position.rotation, glm::vec3(0.0f, 0.0f, -1.0f));
    });

    eventManager.RegisterEvent(InputAction::left_mouse_click, [this, playerEntityID](float _){
        auto playerView = mRegistry.getView<PlayerTagComponent, PositionComponent, CameraComponent>();

        if (playerView.begin() == playerView.end()) {
            return;
        }

        auto [playerEntity, playerTag, position, camera] = *playerView.begin();

        glm::vec3 blockWorldCoords = this->getBlock(mRegistry, position, camera, 0.0001f);
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

        bismuth::EntityID entity = mRegistry.createEntity();

        mRegistry.emplaceComponent<BlockEventComponent>(entity, blockWorldCoords, now);
        mRegistry.emplaceComponent<BlockBreakEventComponent>(entity, playerEntityID);
    });

    eventManager.RegisterEvent(InputAction::right_mouse_click, [this, playerEntityID](float _){
        auto playerView = mRegistry.getView<PlayerTagComponent, PositionComponent, CameraComponent, InventoryComponent>();

        if (playerView.begin() == playerView.end()) {
            return;
        }

        auto [playerEntity, playerTag, position, camera, inventory] = *playerView.begin();

        glm::vec3 blockWorldCoords = this->getBlock(mRegistry, position, camera, -0.0001f);
        BlockTypes currentBlock = inventory.inventory[inventory.currentSlot].item;
        auto now = std::chrono::system_clock::now();

        bismuth::EntityID entity = mRegistry.createEntity();

        mRegistry.emplaceComponent<BlockEventComponent>(entity, blockWorldCoords, now);
        mRegistry.emplaceComponent<BlockPlaceEventComponent>(entity, currentBlock, playerEntityID);
    });

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
            glm::vec2(1,0), // Side
            glm::vec2(1,0), // Side
            glm::vec2(1,0), // Side
            glm::vec2(2,0), // Top
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

glm::vec3 Application::getBlock(
    bismuth::Registry      & registry, 
    PositionComponent const& playerPos, 
    CameraComponent   const& playerCam, 
    float                    epsilon
) {
    Line ray;
    ray.mPosition = playerPos.position;
    ray.mDirection = glm::normalize(playerCam.viewDirection);

    auto chunkView = registry.getView<ChunkTagComponent, BoundingBoxCollectionComponent, PositionComponent>();

    int playerX = static_cast<int>(std::floor(playerPos.position.x/VoxelWorlds::CHUNK_SIZE));
    int playerY = static_cast<int>(std::floor(playerPos.position.y/VoxelWorlds::CHUNK_SIZE));
    int playerZ = static_cast<int>(std::floor(playerPos.position.z/VoxelWorlds::CHUNK_SIZE));

    std::vector<BoundingBoxCollectionComponent*> chunksBoundings;
    for(auto [entity, chunkTag, boundingCollection, position] : chunkView) {
        int chunkX = static_cast<int>(std::floor(position.position.x/VoxelWorlds::CHUNK_SIZE));
        int chunkY = static_cast<int>(std::floor(position.position.y/VoxelWorlds::CHUNK_SIZE));
        int chunkZ = static_cast<int>(std::floor(position.position.z/VoxelWorlds::CHUNK_SIZE));
        
        // Check if this chunk is in the 3x3x3 area around the player
        if (abs(chunkX - playerX) <= 1 && 
            abs(chunkY - playerY) <= 1 && 
            abs(chunkZ - playerZ) <= 1) {
            chunksBoundings.push_back(&boundingCollection);
        }
    }

    float collisionDistance = std::numeric_limits<float>::infinity();
    glm::vec3 globalCoords = glm::vec3(0);

    for(const auto& chunkBounding : chunksBoundings) {
        for(const auto& box : chunkBounding->boundingBoxes) {   
            float distance = physics::LineIntersectsAABB(ray, box);
            
            if(distance != -1 && distance < collisionDistance) {
                collisionDistance = distance;
                globalCoords = ray.mPosition + ray.mDirection * (distance + epsilon);
            }
        }
    }
    
    return (glm::length(globalCoords) != 0) ? globalCoords : playerPos.position;
}