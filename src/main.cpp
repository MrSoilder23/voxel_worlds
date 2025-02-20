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
#include "./blocks/blocks.hpp"
#include "./utility/thread_pool.hpp"
#include "./utility/spiral_loop.hpp"

#include "./components/player_controller_component.hpp"
#include "./components/position_component.hpp"
#include "./components/camera_component.hpp"
#include "./components/bounding_box_component.hpp"

struct Settings {
    GLuint mGraphicsShaderProgram = 0;

    int mScreenWidth = 1366;
    int mScreenHeight = 768;

    float mSpeed = 20.0f;
    float mSensitivity = 24.0f;
};
Settings gSettings;
Game gGame;

std::shared_ptr<GraphicsApp> gGraphicsApp = std::make_shared<GraphicsApp>();
EntityManager& gEntityManager = EntityManager::GetInstance();
PlayerControllerSystem gPlayerControllerSys;

RendererSystem& gRendererSystem = RendererSystem::GetInstance();
ChunkRendererSystem& gChunkRendererSystem = ChunkRendererSystem::GetInstance();
// ChunkManager& gChunkManager = ChunkManager::GetInstance();

// ChunkSystem chunkSystem;
ThreadPool& gThreadPool = ThreadPool::GetInstance();
World gWorld;

void Input(float deltaTime) {
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
}


std::mutex gWorldMutex;

void MainLoop(float deltaTime) {
    static float smoothedFPS = 0.0f;
    static float alpha = 0.1f;  

    float fps = 1.0f/deltaTime;
    smoothedFPS = alpha * fps + (1.0f - alpha) * smoothedFPS;

    std::string newTitle = "Giera, FPS: " + std::to_string(static_cast<int>(smoothedFPS));
    SDL_SetWindowTitle(gGame.GetWindow(), newTitle.data());

    static int delay = 90; // delay in frames
    static SpiralLoop loop;
    static SpiralLoop loop2;

    static float gCameraOldX = 0;
    static float gCameraOldY = 0;
    static float gCameraOldZ = 0;

    // auto chunk = gChunkManager.GetChunk(0,0,0);
    // gChunkRendererSystem.DrawChunk(chunk);
    glm::vec3 camera = gEntityManager.GetComponent<CameraComponent>("Player")->mEye;

    int cameraX = static_cast<int>(std::floor(camera.x/VoxelWorlds::CHUNK_SIZE));
    int cameraY = static_cast<int>(std::floor(camera.y/VoxelWorlds::CHUNK_SIZE));
    int cameraZ = static_cast<int>(std::floor(camera.z/VoxelWorlds::CHUNK_SIZE));
    
    if(cameraX != gCameraOldX || cameraY != gCameraOldY || cameraZ != gCameraOldZ) {
        loop.Reset();
        loop2.Reset();
        delay = 10;
    }

    gCameraOldX = cameraX;
    gCameraOldY = cameraY;
    gCameraOldZ = cameraZ;
    
    {   
        gWorld.SetCameraPosition(camera);
        
        int loopX  = loop.GetLoopX()  + cameraX;
        int loopX1 = loop2.GetLoopX() + cameraX;
        int loopZ  = loop.GetLoopZ()  + cameraZ;
        int loopZ1 = loop2.GetLoopZ() + cameraZ;

        // Temporal fix, need to set better locks
        gThreadPool.enqueue([ptr = &gWorld, loopX, loopZ]() {
            std::lock_guard<std::mutex> lock(gWorldMutex);
            ptr->GenerateChunks(loopX, loopZ);
            ptr->GenerateWorld(loopX, loopZ);
            ptr->GenerateMesh(loopX, loopZ);
        });

        gWorld.WorldVao(loopX1, loopZ1);
        
        gWorld.DrawChunks(gEntityManager);
        
        loop.Loop(VoxelWorlds::RENDER_DISTANCE+VoxelWorlds::CHUNK_GENERATION_OFFSET);
        if(delay <= 0) {
            loop2.Loop(VoxelWorlds::RENDER_DISTANCE+VoxelWorlds::CHUNK_GENERATION_OFFSET);
        } else {
            delay--;
        }
    }

    gRendererSystem.DrawAllDebug();
    gPlayerControllerSys.Update(gEntityManager, deltaTime);
}
GLuint VAO, VBO, EBO;

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

        gEntityManager.GetComponent<PlayerControllerComponent>("Player")->mSensitivity = gSettings.mSensitivity;

        gPlayerControllerSys.SetFov(45.0f);
        gPlayerControllerSys.SetScreenSize(gSettings.mScreenWidth,gSettings.mScreenHeight);
        gPlayerControllerSys.SetCamera(gEntityManager, 0.1f);
    }

    {
        gEntityManager.CreateEntity("Test");
        gEntityManager.AddComponent<BoundingBoxComponent>("Test");
        gEntityManager.AddComponent<ModelComponent>("Test");

        auto boundingBox = gEntityManager.GetComponent<BoundingBoxComponent>("Test");
        boundingBox->mMin = glm::vec3(0.0f,0.0f,0.0f);
        boundingBox->mMax = glm::vec3(1.0f,1.0f,1.0f);

        auto a = gEntityManager.GetComponent<ModelComponent>("Test");

        a->mModel = utility::CreateBoundingModel(*boundingBox);
    }
    {
        gEntityManager.CreateEntity("Test1");
        gEntityManager.AddComponent<BoundingBoxComponent>("Test1");
        gEntityManager.AddComponent<ModelComponent>("Test1");

        auto boundingBox = gEntityManager.GetComponent<BoundingBoxComponent>("Test1");
        boundingBox->mMin = glm::vec3(-0.5f,-0.5f,-0.5f);
        boundingBox->mMax = glm::vec3(0.5f,0.5f,0.5f);

        auto a = gEntityManager.GetComponent<ModelComponent>("Test1");
        
        a->mModel = utility::CreateBoundingModel(*boundingBox);
    }

    VertexSetupSystem vSetupSystem;

    vSetupSystem.CreateVertexSpecification(gEntityManager);
    gRendererSystem.AddGraphicsApp(gGraphicsApp);
    gChunkRendererSystem.AddGraphicsApp(gGraphicsApp);

    gGame.SetEventCallback(Input);
    gGame.SetUpdateCallback(MainLoop);
    
    gGame.RunLoop();

    return 0;
}