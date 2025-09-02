#include "core/application.hpp"

// Public
void Application::run() {
    mGame.InitializeProgram("Giera", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mSettings.screenWidth, mSettings.screenHeight);

    mSettings.mGraphicsShaderProgram = shader::CreateGraphicsPipeline("./shaders/vert.glsl", "./shaders/frag.glsl");
    mGraphicsApp->mGraphicsPipeline = mSettings.mGraphicsShaderProgram;

    initialize();
    initializeRender();

    
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

    static CircleLoop cLoop;

    static float gCameraOldX = 0;
    static float gCameraOldY = 0;
    static float gCameraOldZ = 0;

    glm::vec3 camera = mEntityManager.GetComponent<PositionComponent>("Player")->mPosition;

    int cameraX = static_cast<int>(std::floor(camera.x/VoxelWorlds::CHUNK_SIZE));
    int cameraY = static_cast<int>(std::floor(camera.y/VoxelWorlds::CHUNK_SIZE));
    int cameraZ = static_cast<int>(std::floor(camera.z/VoxelWorlds::CHUNK_SIZE));
    
    constexpr int generationSpeed = 2;

    static int radius = generationSpeed;
    static int i = 0;
    cLoop.SetCenter(cameraX, cameraZ);
    static std::vector<std::pair<int, int>> coords = cLoop.Loop(radius);

    if(cameraX != gCameraOldX || cameraY != gCameraOldY || cameraZ != gCameraOldZ) {
        radius = generationSpeed;
    }

    gCameraOldX = cameraX;
    gCameraOldY = cameraY;
    gCameraOldZ = cameraZ;

    {   
    
        if(mSettings.worldGen) {
            for(int iSpeed = 0; iSpeed < 6; iSpeed++) {
                i += (iSpeed == 0) ? 0 : 1;

                if(i >= coords.size()) {
                    continue;
                }

                int loopX = coords[i].first;
                int loopY = cameraY;
                int loopZ = coords[i].second;

                for(int y = -VoxelWorlds::RENDER_DISTANCE; y < VoxelWorlds::RENDER_DISTANCE; y++) {
                    int newY = loopY + y;
                    mWorldGen.GenerateChunk(loopX, newY, loopZ);
                }
            }
        }

        if(i < coords.size()) {
            i++;
        } else {
            if(radius < VoxelWorlds::RENDER_DISTANCE+VoxelWorlds::CHUNK_GENERATION_OFFSET) {
                radius += std::min(generationSpeed, VoxelWorlds::RENDER_DISTANCE+VoxelWorlds::CHUNK_GENERATION_OFFSET-radius);
            } else {
                radius = generationSpeed;
            }
            i = 0;
            coords = cLoop.Loop(radius);
        }
    }
}
void Application::system(float deltaTime) {
    static PositionUpdateSystem posUpdateSystem;
    static VertexSetupSystem vSetupSystem;
    static BoundingBoxSystem boundingBoxSystem;
    static CollisionSystem collisionSystem;
    static PhysicsSystem physSystem;
    static BlockEventSystem blockEventSystem;
    static ChunkMeshingSystem chunkMeshSystem;
    static ChunkVertexSetupSystem chunkVSS;
    static ChunkUnloadSystem chunkUnloadSystem;
    static ChunkCreationSystem chunkCreationSystem;

    mPlayerControllerSys.Update(mEntityManager);

    chunkCreationSystem.CreateChunkData(mEntityManager, mSeed);
    chunkUnloadSystem.UnloadChunks(mEntityManager);

    if(mSettings.physics) {
        collisionSystem.UpdateCollision(mEntityManager, deltaTime);
    }

    physSystem.UpdatePosition(mEntityManager, deltaTime);
    posUpdateSystem.UpdatePositionTransform(mEntityManager);

    blockEventSystem.UpdateChunks(mEntityManager);

    chunkMeshSystem.CreateChunksMesh(mEntityManager);
    
    boundingBoxSystem.GenerateBoundingBox(mEntityManager);
    vSetupSystem.CreateVertexSpecification(mEntityManager);
    chunkVSS.CreateVertexSpecification(mEntityManager);

    mRendererSystem.DrawAll(mEntityManager);
    if(mSettings.boundingDebug) {
        mRendererSystem.DrawAllDebug(mEntityManager);
    }
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
    initializeModels();
    initializeTextures();
    initializeBlocks();

    initializeWorld();

    initializeBaseEntities();
    initializeKeys();
}
void Application::initializeWorld() {
    static std::random_device rndDevice;
    mSeed = rndDevice();

    mWorldGen.SetEntityManager(mEntityManager);
    mChunkbBoxCreationSys.SetEntityManager(mEntityManager);
    mWorldGen.SetSeed(mSeed);
}
void Application::initializeKeys() {
    static PlayerTargetSystem pTarget;

    mEventManager.RegisterEvent(InputAction::exit, [game = &mGame](float _){game->StopLoop();});
    mEventManager.RegisterEvent(InputAction::toggle_debug, [settings = &mSettings](float _){settings->physics = !settings->physics;});
    mEventManager.RegisterEvent(InputAction::toggle_debug2, [settings = &mSettings](float _){settings->boundingDebug = !settings->boundingDebug;});
    mEventManager.RegisterEvent(InputAction::toggle_debug3, [settings = &mSettings](float _){settings->worldGen = !settings->worldGen;});

    pTarget.PlayerRaycast(mEntityManager);
}
void Application::initializeBaseEntities() {
    // Player Entity
    mEntityManager.CreateEntity("Player");
    mEntityManager.AddComponent<PlayerControllerComponent>("Player");
    mEntityManager.AddComponent<PositionComponent>("Player");
    mEntityManager.AddComponent<CameraComponent>("Player");
    mEntityManager.AddComponent<BoundingBoxComponent>("Player");
    mEntityManager.AddComponent<PhysicsComponent>("Player");
    mEntityManager.AddComponent<InventoryComponent>("Player");

    auto player = mEntityManager.GetComponent<PlayerControllerComponent>("Player");
    player->mSensitivity = mSettings.sensitivity;
    player->mSpeed = mSettings.speed;

    mPlayerControllerSys.SetFov(45.0f);
    mPlayerControllerSys.SetScreenSize(mSettings.screenWidth,mSettings.screenHeight);
    mPlayerControllerSys.SetCamera(mEntityManager, 0.01f);
    mPlayerControllerSys.InitializeMovement(mEntityManager);

    auto playerPosition = mEntityManager.GetComponent<PositionComponent>("Player");
    playerPosition->mPosition.y = world_generation::GenerateHeight(mSeed, 0,0)+1.2f;
    
    auto playerBox = mEntityManager.GetComponent<BoundingBoxComponent>("Player");
    playerBox->mLocalMin = glm::vec3(-0.4, -1.5, -0.4);
    playerBox->mLocalMax = glm::vec3( 0.4,  0.4,  0.4);

    auto playerPhysics = mEntityManager.GetComponent<PhysicsComponent>("Player");
    playerPhysics->mFriction = 2.0f;
}
void Application::initializeRender() {
    mRendererSystem.AddGraphicsApp(mGraphicsApp);
}
void Application::initializeModels() {
    ModelManager& modelManager = ModelManager::GetInstance();

    Model block;

    block.vertexPositions = {
        // Front Face
        glm::vec3(-0.5f, -0.5f,  0.5f), // 0  - Bottom-left
        glm::vec3( 0.5f, -0.5f,  0.5f), // 1  - Bottom-right
        glm::vec3(-0.5f,  0.5f,  0.5f), // 2  - Top-left
        glm::vec3( 0.5f,  0.5f,  0.5f), // 3  - Top-right

        // Back Face
        glm::vec3(-0.5f, -0.5f, -0.5f), // 4  - Bottom-left
        glm::vec3( 0.5f, -0.5f, -0.5f), // 5  - Bottom-right
        glm::vec3(-0.5f,  0.5f, -0.5f), // 6  - Top-left
        glm::vec3( 0.5f,  0.5f, -0.5f), // 7  - Top-right

        // Right Face
        glm::vec3( 0.5f, -0.5f,  0.5f), //8
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f), //11

        // Left Face
        glm::vec3(-0.5f, -0.5f, -0.5f), //12
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f), //15

        // Top Face
        glm::vec3(-0.5f,  0.5f,  0.5f), //16
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f), //19

        // Bottom Face
        glm::vec3(-0.5f, -0.5f, -0.5f), //20
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f), //23
    };

    // block.indexBufferData = {
    //     0,1,2, 3,2,1, // FrontFace
    //     0,4,5, 1,0,5, // BottomFace
    //     6,7,4, 7,5,4, // BackFace
    //     6,2,7, 3,7,2, // TopFace
    //     3,1,7, 5,7,1, // RightFace
    //     6,4,2, 0,2,4, // LeftFace
    // };

    block.indexBufferData = {};

    modelManager.CreateNewModel("cube", block);
}
void Application::initializeTextures() {
    TextureManager& textureManager = TextureManager::GetInstance();
    BlockTextureCreator& blockTextureCreator = BlockTextureCreator::GetInstance();

    SDL_Surface* chunkAtlas = IMG_Load("./assets/AtlasMap.png");


    if (!chunkAtlas) {
        std::cerr << "Failed to load textures!" << std::endl;
        return;
    }

    textureManager.CreateNewTexture("ChunkTexture", chunkAtlas);

    blockTextureCreator.CreateTexture("ChunkTexture", chunkAtlas);
}
void Application::initializeBlocks() {
    BlockRegistry& blockRegistry = BlockRegistry::GetInstance();
    ModelManager& modelManager = ModelManager::GetInstance();

    // Grass block
    BlockTemplate grassBlock;

    grassBlock.textureCoords[0] = glm::vec2(1,0);
    grassBlock.textureCoords[1] = glm::vec2(1,0);
    grassBlock.textureCoords[2] = glm::vec2(1,0);
    grassBlock.textureCoords[3] = glm::vec2(1,0);
    grassBlock.textureCoords[4] = glm::vec2(2,0);
    grassBlock.textureCoords[5] = glm::vec2(0,0);

    grassBlock.model = modelManager.GetModel("cube");
    blockRegistry.RegisterBlock(BlockTypes::grass_block, grassBlock);

    // Dirt Block    
    BlockTemplate dirtBlock;

    dirtBlock.textureCoords[0] = glm::vec2(0,0);
    dirtBlock.textureCoords[1] = glm::vec2(0,0);
    dirtBlock.textureCoords[2] = glm::vec2(0,0);
    dirtBlock.textureCoords[3] = glm::vec2(0,0);
    dirtBlock.textureCoords[4] = glm::vec2(0,0);
    dirtBlock.textureCoords[5] = glm::vec2(0,0);

    dirtBlock.model = modelManager.GetModel("cube");
    blockRegistry.RegisterBlock(BlockTypes::dirt_block, dirtBlock);

    // Stone Block    
    BlockTemplate stoneBlock;

    stoneBlock.textureCoords[0] = glm::vec2(4,0);
    stoneBlock.textureCoords[1] = glm::vec2(4,0);
    stoneBlock.textureCoords[2] = glm::vec2(4,0);
    stoneBlock.textureCoords[3] = glm::vec2(4,0);
    stoneBlock.textureCoords[4] = glm::vec2(4,0);
    stoneBlock.textureCoords[5] = glm::vec2(4,0);

    stoneBlock.model = modelManager.GetModel("cube");
    blockRegistry.RegisterBlock(BlockTypes::stone_block, stoneBlock);

    // Sand Block    
    BlockTemplate sandBlock;

    sandBlock.textureCoords[0] = glm::vec2(3,0);
    sandBlock.textureCoords[1] = glm::vec2(3,0);
    sandBlock.textureCoords[2] = glm::vec2(3,0);
    sandBlock.textureCoords[3] = glm::vec2(3,0);
    sandBlock.textureCoords[4] = glm::vec2(3,0);
    sandBlock.textureCoords[5] = glm::vec2(3,0);

    sandBlock.model = modelManager.GetModel("cube");
    blockRegistry.RegisterBlock(BlockTypes::sand_block, sandBlock);
    
}
