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

// Own libraries
#include "./graphics/shader.hpp"
#include "./graphics/graphics.hpp"
#include "./core/entity_manager.hpp"
#include "./core/world.hpp"
#include "./core/game.hpp"

#include "./systems/renderer_system.hpp"
#include "./systems/chunk_renderer_system.hpp"
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

#include "./blocks/blocks.hpp"
#include "./utility/thread_pool.hpp"
#include "./utility/spiral_loop.hpp"

#include "./components/player_controller_component.hpp"
#include "./components/position_component.hpp"
#include "./components/camera_component.hpp"
#include "./components/bounding_box_component.hpp"
#include "./components/physics_component.hpp"

struct Settings {
    GLuint mGraphicsShaderProgram = 0;

    int mScreenWidth = 1366;
    int mScreenHeight = 768;

    float mSpeed = 100.0f;
    float mSensitivity = 24.0f;

    bool mBoundingDebug = false;
};
Settings gSettings;
Game gGame;

std::shared_ptr<GraphicsApp> gGraphicsApp = std::make_shared<GraphicsApp>();
EntityManager& gEntityManager = EntityManager::GetInstance();
PlayerControllerSystem gPlayerControllerSys;
ChunkBoundingCreationSystem gChunkbBoxCreationSys;

RendererSystem& gRendererSystem = RendererSystem::GetInstance();
ChunkRendererSystem& gChunkRendererSystem = ChunkRendererSystem::GetInstance();
// ChunkManager& gChunkManager = ChunkManager::GetInstance();

// ChunkSystem chunkSystem;
ThreadPool& gThreadPool = ThreadPool::GetInstance();
WorldGenerationSystem gWorldGen;
World gWorld;

void Input(float deltaTime) {
    gPlayerControllerSys.Update(gEntityManager, deltaTime);

    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            gGame.StopLoop();
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]) {
        gGame.StopLoop();
    }
    if(state[SDL_SCANCODE_F12]) {
        gSettings.mBoundingDebug = !gSettings.mBoundingDebug;
    }
}

void MainLoop(float deltaTime) {
    static uint8_t delay = 90;
    static float smoothedFPS = 0.0f;
    static float alpha = 0.1f;  

    float fps = 1.0f/deltaTime;
    smoothedFPS = alpha * fps + (1.0f - alpha) * smoothedFPS;

    std::string newTitle = "Giera, FPS: " + std::to_string(static_cast<int>(smoothedFPS));
    SDL_SetWindowTitle(gGame.GetWindow(), newTitle.data());

    static SpiralLoop loop;
    static SpiralLoop loop1;

    static float gCameraOldX = 0;
    static float gCameraOldY = 0;
    static float gCameraOldZ = 0;

    static int yLoop = VoxelWorlds::RENDER_DISTANCE;

    glm::vec3 camera = gEntityManager.GetComponent<PositionComponent>("Player")->mPosition;

    int cameraX = static_cast<int>(std::floor(camera.x/VoxelWorlds::CHUNK_SIZE));
    int cameraY = static_cast<int>(std::floor(camera.y/VoxelWorlds::CHUNK_SIZE));
    int cameraZ = static_cast<int>(std::floor(camera.z/VoxelWorlds::CHUNK_SIZE));
    
    if(cameraX != gCameraOldX || cameraY != gCameraOldY || cameraZ != gCameraOldZ) {
        loop.Reset();
        loop1.Reset();
        delay = 30;
    }

    gCameraOldX = cameraX;
    gCameraOldY = cameraY;
    gCameraOldZ = cameraZ;

    {   
        gWorld.SetCameraPosition(camera);
        
        int loopX = loop.GetLoopX() + cameraX;
        int loopY = yLoop + cameraY;
        int loopZ = loop.GetLoopZ() + cameraZ;

        int loopX1 = loop1.GetLoopX() + cameraX;
        int loopZ1 = loop1.GetLoopZ() + cameraZ;
        
        // gThreadPool.enqueue([ptr = &gWorldGen, bBox = &gChunkbBoxCreationSys, loopX, loopY, loopZ]() {
        //     // std::lock_guard<std::mutex> lock(gWorldMutex);
        //     for(int y = VoxelWorlds::RENDER_DISTANCE; y > -VoxelWorlds::RENDER_DISTANCE; y--) {
        //         int newY = loopY + y;
        //         ptr->GenerateChunk(loopX, newY, loopZ);
        //         ptr->GenerateModel(loopX, newY, loopZ);
        //         // bBox->CreateChunkBoundingBoxes(loopX, newY, loopZ);
        //     }
        // });

        gWorldGen.GenerateChunk(loopX, loopY, loopZ);
        gWorldGen.GenerateModel(loopX, loopY, loopZ);

            // if(delay <= 1) {
            //     gWorldGen.GenerateModel(loopX1, loopY, loopZ1);
            // }
            
        if(yLoop <= -VoxelWorlds::RENDER_DISTANCE) {
            loop.Loop(VoxelWorlds::RENDER_DISTANCE+VoxelWorlds::CHUNK_GENERATION_OFFSET);
            yLoop = VoxelWorlds::RENDER_DISTANCE;
            if(delay <= 1) {
                loop1.Loop(VoxelWorlds::RENDER_DISTANCE+VoxelWorlds::CHUNK_GENERATION_OFFSET);
            } else {
                delay--;
            }
        } else {
            yLoop--;
        }
 

    }

    static PositionUpdateSystem posUpdateSystem;
    static VertexSetupSystem vSetupSystem;
    static BoundingBoxSystem boundingBoxSystem;
    static ChunkVertexSetupSystem chunkVSS;
    static CollisionSystem collisionSystem;
    static PhysicsSystem physSystem;
    static PlayerTargetSystem pTarget;
    
    if(gSettings.mBoundingDebug) {
        collisionSystem.UpdateCollision(gEntityManager, deltaTime);
        pTarget.PlayerRaycast(gEntityManager);
    }

    physSystem.UpdatePosition(gEntityManager, deltaTime);
    posUpdateSystem.UpdatePositionTransform(gEntityManager);
    
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

    InitializeModels();
    InitializeTextures();
    InitializeBlocks();

    static std::random_device rndDevice;
    unsigned int seed = rndDevice();

    gWorld.SetSeed(seed);
    gWorld.SetRenderDistance(VoxelWorlds::RENDER_DISTANCE);

    {
        gEntityManager.CreateEntity("Player");
        gEntityManager.AddComponent<PlayerControllerComponent>("Player");
        gEntityManager.AddComponent<PositionComponent>("Player");
        gEntityManager.AddComponent<CameraComponent>("Player");
        gEntityManager.AddComponent<BoundingBoxComponent>("Player");
        gEntityManager.AddComponent<PhysicsComponent>("Player");

        auto player = gEntityManager.GetComponent<PlayerControllerComponent>("Player");
        player->mSensitivity = gSettings.mSensitivity;
        player->mSpeed = gSettings.mSpeed;

        auto playerBox = gEntityManager.GetComponent<BoundingBoxComponent>("Player");

        gPlayerControllerSys.SetFov(45.0f);
        gPlayerControllerSys.SetScreenSize(gSettings.mScreenWidth,gSettings.mScreenHeight);
        gPlayerControllerSys.SetCamera(gEntityManager, 0.01f);

        playerBox->mLocalMin = glm::vec3(-0.4, -1.5, -0.4);
        playerBox->mLocalMax = glm::vec3( 0.4,  0.4,  0.4);
    }

    {
        gEntityManager.CreateEntity("Test");
        gEntityManager.AddComponent<BoundingBoxComponent>("Test");
        gEntityManager.AddComponent<PositionComponent>("Test");

        auto boundingBox = gEntityManager.GetComponent<BoundingBoxComponent>("Test");
        boundingBox->mLocalMin = glm::vec3(0.0f,0.0f,0.0f);
        boundingBox->mLocalMax = glm::vec3(1.0f,1.0f,1.0f);
    }
    {
        gEntityManager.CreateEntity("Test1");
        gEntityManager.AddComponent<BoundingBoxComponent>("Test1");
        gEntityManager.AddComponent<PositionComponent>("Test1");

        auto boundingBox = gEntityManager.GetComponent<BoundingBoxComponent>("Test1");
        boundingBox->mLocalMin = glm::vec3(-0.5f,-1.5f,-0.5f);
        boundingBox->mLocalMax = glm::vec3(0.5f,0.4f,0.5f);
    }

    gWorldGen.SetEntityManager(gEntityManager);
    gChunkbBoxCreationSys.SetEntityManager(gEntityManager);
    gWorldGen.SetSeed(seed);

    gRendererSystem.AddGraphicsApp(gGraphicsApp);
    gChunkRendererSystem.AddGraphicsApp(gGraphicsApp);

    gGame.SetEventCallback(Input);
    gGame.SetUpdateCallback(MainLoop);
    
    gGame.RunLoop();

    return 0;
}