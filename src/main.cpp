// C++ Standard libraries
#include <iostream>
#include <string>
#include <memory>
#include <random>
#include <mutex>

// Third_party libraries
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <tbb/tbb.h>

// Own libraries
#include "./graphics/shader.hpp"
#include "./core/entity_manager.hpp"
#include "./core/game.hpp"

#include "./systems/renderer_system.hpp"
#include "./systems/player_controller_system.hpp"
#include "./systems/vertex_setup_system.hpp"
#include "./systems/bounding_box_system.hpp"
#include "./systems/position_update_system.hpp"
#include "./systems/chunk_vertex_setup_system.hpp"
#include "./systems/world_generation_system.hpp"
#include "./systems/chunk_bounding_creation_system.hpp"
#include "./systems/collision_system.hpp"
#include "./systems/physics_system.hpp"
#include "./systems/player_target_system.hpp"
#include "./systems/chunk_meshing_system.hpp"
#include "./systems/block_event_system.hpp"
#include "./systems/chunk_unload_system.hpp"
#include "./systems/chunk_creation_system.hpp"

#include "./blocks/blocks.hpp"
#include "./utility/thread_pool.hpp"
#include "./utility/spiral_loop.hpp"
#include "./utility/circle_loop.hpp"
#include "./core/event_manager.hpp"
#include "./graphics/graphics.hpp"

#include "./components/player_controller_component.hpp"
#include "./components/position_component.hpp"
#include "./components/camera_component.hpp"
#include "./components/bounding_box_component.hpp"
#include "./components/physics_component.hpp"
#include "./components/inventory_component.hpp"

struct Settings {
    GLuint mGraphicsShaderProgram = 0;

    int mScreenWidth = 1366;
    int mScreenHeight = 768;

    float mSpeed = 100.0f;
    float mSensitivity = 24.0f;

    bool mBoundingDebug = false;
    bool mPhysics = true;
    bool mWorldGen = true;
};
Settings gSettings;
Game gGame;

std::shared_ptr<GraphicsApp> gGraphicsApp = std::make_shared<GraphicsApp>();
EntityManager& gEntityManager = EntityManager::GetInstance();
PlayerControllerSystem gPlayerControllerSys;
ChunkBoundingCreationSystem gChunkbBoxCreationSys;

RendererSystem& gRendererSystem = RendererSystem::GetInstance();
// ChunkManager& gChunkManager = ChunkManager::GetInstance();

// ChunkSystem chunkSystem;
ThreadPool& gThreadPool = ThreadPool::GetInstance();
WorldGenerationSystem gWorldGen;

EventManager& gEventManager = EventManager::GetInstance();
tbb::task_arena gArena;

unsigned int gSeed;

// Initialization
void InitializeKeys() {
    static PlayerTargetSystem pTarget;

    gEventManager.RegisterEvent(InputAction::exit, [game = &gGame](float _){game->StopLoop();});
    gEventManager.RegisterEvent(InputAction::toggle_debug, [settings = &gSettings](float _){settings->mPhysics = !settings->mPhysics;});
    gEventManager.RegisterEvent(InputAction::toggle_debug2, [settings = &gSettings](float _){settings->mBoundingDebug = !settings->mBoundingDebug;});
    gEventManager.RegisterEvent(InputAction::toggle_debug3, [settings = &gSettings](float _){settings->mWorldGen = !settings->mWorldGen;});

    pTarget.PlayerRaycast(gEntityManager);
}

void InitializeBaseEntities() {
    // Player Entity
    gEntityManager.CreateEntity("Player");
    gEntityManager.AddComponent<PlayerControllerComponent>("Player");
    gEntityManager.AddComponent<PositionComponent>("Player");
    gEntityManager.AddComponent<CameraComponent>("Player");
    gEntityManager.AddComponent<BoundingBoxComponent>("Player");
    gEntityManager.AddComponent<PhysicsComponent>("Player");
    gEntityManager.AddComponent<InventoryComponent>("Player");

    auto player = gEntityManager.GetComponent<PlayerControllerComponent>("Player");
    player->mSensitivity = gSettings.mSensitivity;
    player->mSpeed = gSettings.mSpeed;

    gPlayerControllerSys.SetFov(45.0f);
    gPlayerControllerSys.SetScreenSize(gSettings.mScreenWidth,gSettings.mScreenHeight);
    gPlayerControllerSys.SetCamera(gEntityManager, 0.01f);
    gPlayerControllerSys.InitializeMovement(gEntityManager);

    auto playerPosition = gEntityManager.GetComponent<PositionComponent>("Player");
    playerPosition->mPosition.y = world_generation::GenerateHeight(gSeed, 0,0)+1.2f;
    
    auto playerBox = gEntityManager.GetComponent<BoundingBoxComponent>("Player");
    playerBox->mLocalMin = glm::vec3(-0.4, -1.5, -0.4);
    playerBox->mLocalMax = glm::vec3( 0.4,  0.4,  0.4);

    auto playerPhysics = gEntityManager.GetComponent<PhysicsComponent>("Player");
    playerPhysics->mFriction = 2.0f;
}

void InitializeWorld() {
    static std::random_device rndDevice;
    gSeed = rndDevice();

    gWorldGen.SetEntityManager(gEntityManager);
    gChunkbBoxCreationSys.SetEntityManager(gEntityManager);
    gWorldGen.SetSeed(gSeed);
}

void Initialize() {
    InitializeModels();
    InitializeTextures();
    InitializeBlocks();

    InitializeWorld();

    InitializeBaseEntities();
    InitializeKeys();
}

void InitializeRender() {
    gRendererSystem.AddGraphicsApp(gGraphicsApp);
}

void Input(float deltaTime) {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            gGame.StopLoop();
        }
        if(e.type == SDL_KEYDOWN) {
            if(e.key.keysym.sym == SDLK_ESCAPE) {
                gGame.StopLoop();
            }

            if (e.key.keysym.sym == SDLK_F10) {
                gEventManager.GetEvent(InputAction::toggle_debug3, deltaTime);
            }
            if (e.key.keysym.sym == SDLK_F11) {
                gEventManager.GetEvent(InputAction::toggle_debug, deltaTime);
            }
            if (e.key.keysym.sym == SDLK_F12) {
                gEventManager.GetEvent(InputAction::toggle_debug2, deltaTime);
            }
        }
        if(e.type == SDL_MOUSEMOTION) {
            int mouseX = e.motion.xrel;
            int mouseY = e.motion.yrel;
            
            gEventManager.GetMouseMotionEvent(InputAction::mouse_motion, deltaTime, mouseX, mouseY);
        }
        if(e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                gEventManager.GetEvent(InputAction::left_mouse_click, deltaTime);
            }
            if(e.button.button == SDL_BUTTON_RIGHT) {
                gEventManager.GetEvent(InputAction::right_mouse_click, deltaTime);
            }
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_W]) {
        gEventManager.GetEvent(InputAction::move_forward, deltaTime);
    }
    if(state[SDL_SCANCODE_A]) {
        gEventManager.GetEvent(InputAction::move_left, deltaTime);
    }
    if(state[SDL_SCANCODE_S]) {
        gEventManager.GetEvent(InputAction::move_backwards, deltaTime);
    }
    if(state[SDL_SCANCODE_D]) {
        gEventManager.GetEvent(InputAction::move_right, deltaTime);
    }
    if(state[SDL_SCANCODE_LSHIFT]) {
        gEventManager.GetEvent(InputAction::move_down, deltaTime);
    }
    if(state[SDL_SCANCODE_SPACE]) {
        gEventManager.GetEvent(InputAction::move_up, deltaTime);
    }

    if(state[SDL_SCANCODE_1]) {
        gEventManager.GetEvent(InputAction::hotbar_0, deltaTime);
    }
    if(state[SDL_SCANCODE_2]) {
        gEventManager.GetEvent(InputAction::hotbar_1, deltaTime);
    }
    if(state[SDL_SCANCODE_3]) {
        gEventManager.GetEvent(InputAction::hotbar_2, deltaTime);
    }
    if(state[SDL_SCANCODE_4]) {
        gEventManager.GetEvent(InputAction::hotbar_3, deltaTime);
    }
    if(state[SDL_SCANCODE_5]) {
        gEventManager.GetEvent(InputAction::hotbar_4, deltaTime);
    }
    if(state[SDL_SCANCODE_6]) {
        gEventManager.GetEvent(InputAction::hotbar_5, deltaTime);
    }
    if(state[SDL_SCANCODE_7]) {
        gEventManager.GetEvent(InputAction::hotbar_6, deltaTime);
    }
    if(state[SDL_SCANCODE_8]) {
        gEventManager.GetEvent(InputAction::hotbar_7, deltaTime);
    }
    if(state[SDL_SCANCODE_9]) {
        gEventManager.GetEvent(InputAction::hotbar_8, deltaTime);
    }
    if(state[SDL_SCANCODE_0]) {
        gEventManager.GetEvent(InputAction::hotbar_9, deltaTime);
    }

}

void FpsCounter(float deltaTime) {
    static float smoothedFPS = 0.0f;
    static float alpha = 0.1f;  

    float fps = 1.0f/deltaTime;
    smoothedFPS = alpha * fps + (1.0f - alpha) * smoothedFPS;

    std::string newTitle = "Giera, FPS: " + std::to_string(static_cast<int>(smoothedFPS));
    SDL_SetWindowTitle(gGame.GetWindow(), newTitle.data());
}

void MainLoop(float deltaTime) {
    FpsCounter(deltaTime);

    static CircleLoop cLoop;

    static float gCameraOldX = 0;
    static float gCameraOldY = 0;
    static float gCameraOldZ = 0;

    glm::vec3 camera = gEntityManager.GetComponent<PositionComponent>("Player")->mPosition;

    int cameraX = static_cast<int>(std::floor(camera.x/VoxelWorlds::CHUNK_SIZE));
    int cameraY = static_cast<int>(std::floor(camera.y/VoxelWorlds::CHUNK_SIZE));
    int cameraZ = static_cast<int>(std::floor(camera.z/VoxelWorlds::CHUNK_SIZE));
    
    constexpr int generationSpeed = 6;

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
        
        int loopX = coords[i].first;
        int loopY = cameraY;
        int loopZ = coords[i].second;

        if(gSettings.mWorldGen) {
            for(int y = -VoxelWorlds::RENDER_DISTANCE; y < VoxelWorlds::RENDER_DISTANCE; y++) {
                int newY = loopY + y;
                gWorldGen.GenerateChunk(loopX, newY, loopZ);
            }
        }

        if(i < coords.size()) {
            i++;
        } else {
            if(radius < VoxelWorlds::RENDER_DISTANCE+VoxelWorlds::CHUNK_GENERATION_OFFSET) {
                radius += generationSpeed;
            } else {
                radius = generationSpeed;
            }
            i = 0;
            coords = cLoop.Loop(radius);
        }
    }
}

void System(float deltaTime) {
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

    gPlayerControllerSys.Update(gEntityManager);

    chunkCreationSystem.CreateChunkData(gEntityManager, gSeed);
    chunkUnloadSystem.UnloadChunks(gEntityManager);

    if(gSettings.mPhysics) {
        collisionSystem.UpdateCollision(gEntityManager, deltaTime);
    }

    physSystem.UpdatePosition(gEntityManager, deltaTime);
    posUpdateSystem.UpdatePositionTransform(gEntityManager);

    blockEventSystem.UpdateChunks(gEntityManager);

    chunkMeshSystem.CreateChunksMesh(gEntityManager);
    
    boundingBoxSystem.GenerateBoundingBox(gEntityManager);
    vSetupSystem.CreateVertexSpecification(gEntityManager);
    chunkVSS.CreateVertexSpecification(gEntityManager);

    gRendererSystem.DrawAll(gEntityManager);
    if(gSettings.mBoundingDebug) {
        gRendererSystem.DrawAllDebug(gEntityManager);
    }
}

int main() {

    gGame.InitializeProgram("Giera", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gSettings.mScreenWidth, gSettings.mScreenHeight);

    shader::CreateGraphicsPipeline(gSettings.mGraphicsShaderProgram, "./shaders/vert.glsl", "./shaders/frag.glsl");
    gGraphicsApp->mGraphicsPipeline = gSettings.mGraphicsShaderProgram;

    Initialize();
    InitializeRender();

    gGame.SetEventCallback(Input);
    gGame.SetUpdateCallback(MainLoop);
    gGame.SetSystemCallback(System);
    
    gGame.RunLoop();

    return 0;
}