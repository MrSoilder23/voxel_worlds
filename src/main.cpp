// C++ Standard libraries
#include <iostream>
#include <string>
#include <memory>
#include <random>

// Third_party libraries
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

// Own libraries
#include "./graphics/shader.hpp"
#include "./core/game.hpp"
#include "./graphics/graphics.hpp"
#include "./systems/renderer_system.hpp"
#include "./blocks/blocks.hpp"

#include "./systems/chunk_renderer_system.hpp"
#include "./world/chunk_manager.hpp"
#include "./core/world.hpp"
#include "./utility/thread_pool.hpp"
#include "./utility/spiral_loop.hpp"

struct Settings {
    GLuint mGraphicsShaderProgram = 0;

    int mScreenWidth = 1366;
    int mScreenHeight = 768;

    float mSpeed = 20.0f;
    float mSensitivity = 10.0f;
};
Settings gSettings;
Game gGame;

std::shared_ptr<GraphicsApp> gGraphicsApp = std::make_shared<GraphicsApp>();
EntityManager& gEntityManager = EntityManager::GetInstance();

RendererSystem& gRendererSystem = RendererSystem::GetInstance();
ChunkRendererSystem& gChunkRendererSystem = ChunkRendererSystem::GetInstance();
// ChunkManager& gChunkManager = ChunkManager::GetInstance();

// ChunkSystem chunkSystem;
ThreadPool& threadPool = ThreadPool::GetInstance();
World world;

float gCameraOldX = 0;
float gCameraOldY = 0;
float gCameraOldZ = 0;
SpiralLoop loop;

void Input(float deltaTime) {
    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            gGame.StopLoop();
        }
        if(e.type == SDL_MOUSEMOTION) {
            int mouseX = e.motion.xrel;
            int mouseY = e.motion.yrel;

            gGraphicsApp->mCamera.MouseLook(mouseX, mouseY, gSettings.mSensitivity, deltaTime);
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]) {
        gGame.StopLoop();
    }
    if(state[SDL_SCANCODE_W]) {
        gGraphicsApp->mCamera.MoveForward(gSettings.mSpeed, deltaTime);
    }
    if(state[SDL_SCANCODE_S]) {
        gGraphicsApp->mCamera.MoveBackward(gSettings.mSpeed, deltaTime);
    }
    if(state[SDL_SCANCODE_A]) {
        gGraphicsApp->mCamera.MoveLeft(gSettings.mSpeed, deltaTime);
    }
    if(state[SDL_SCANCODE_D]) {
        gGraphicsApp->mCamera.MoveRight(gSettings.mSpeed, deltaTime);
    }
    if(state[SDL_SCANCODE_SPACE]) {
        gGraphicsApp->mCamera.MoveUp(gSettings.mSpeed, deltaTime);
    }
    if(state[SDL_SCANCODE_LSHIFT]) {
        gGraphicsApp->mCamera.MoveDown(gSettings.mSpeed, deltaTime);
    }


}

void MainLoop(float deltaTime) {
    float fps = 1.0f/deltaTime;
    std::string newTitle = "Giera, FPS: " + std::to_string(fps);
    SDL_SetWindowTitle(gGame.GetWindow(), newTitle.data());

    static int delay = 50; // delay in frames
    static SpiralLoop loop2;
    // auto chunk = gChunkManager.GetChunk(0,0,0);
    // gChunkRendererSystem.DrawChunk(chunk);
    glm::vec3 camera = gGraphicsApp->mCamera.GetEye();

    float cameraX = std::floor(camera.x/VoxelWorlds::CHUNK_SIZE);
    float cameraY = std::floor(camera.y/VoxelWorlds::CHUNK_SIZE);
    float cameraZ = std::floor(camera.z/VoxelWorlds::CHUNK_SIZE);
    
    if(cameraX != gCameraOldX || cameraY != gCameraOldY || cameraZ != gCameraOldZ) {
        loop.Reset();
        loop2.Reset();
    }

    gCameraOldX = cameraX;
    gCameraOldY = cameraY;
    gCameraOldZ = cameraZ;
    
    {   
        world.SetCameraPosition(camera);
        
        int loopX = loop.GetLoopX() + cameraX;
        int loopX1 = loop2.GetLoopX() + cameraX;
        int loopZ = loop.GetLoopZ() + cameraZ;
        int loopZ1 = loop2.GetLoopZ() + cameraZ;

        world.GenerateChunks(loopX, loopZ);

        threadPool.enqueue([ptr = &world, loopX, loopZ]() {
            ptr->GenerateWorld(loopX, loopZ);
        });
        threadPool.enqueue([ptr = &world, loopX, loopZ]() {
            ptr->GenerateMesh(loopX, loopZ);
        });

        world.WorldVao(loopX1, loopZ1);
        world.DrawChunks();

        loop.Loop(VoxelWorlds::RENDER_DISTANCE+VoxelWorlds::CHUNK_GENERATION_OFFSET);
        if(delay <= 0) {
            loop2.Loop(VoxelWorlds::RENDER_DISTANCE+VoxelWorlds::CHUNK_GENERATION_OFFSET);
        } else {
            delay--;
        }
    }
}

int main() {

    gGame.InitializeProgram("Giera", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gSettings.mScreenWidth, gSettings.mScreenHeight);
    gGraphicsApp->mCamera.SetProjectionMatrix(glm::radians(45.0f), (float)gSettings.mScreenWidth/(float)gSettings.mScreenHeight, 0.1f);

    shader::CreateGraphicsPipeline(gSettings.mGraphicsShaderProgram, "./shaders/vert.glsl", "./shaders/frag.glsl");
    gGraphicsApp->mGraphicsPipeline = gSettings.mGraphicsShaderProgram;

    InitializeModels();
    InitializeTextures();
    InitializeBlocks();

    static std::random_device rndDevice;
    unsigned int seed = rndDevice();

    world.SetSeed(seed);
    world.SetRenderDistance(VoxelWorlds::RENDER_DISTANCE);

    // gRendererSystem.AddGraphicsApp(gGraphicsApp);
    gChunkRendererSystem.AddGraphicsApp(gGraphicsApp);

    gGame.SetEventCallback(Input);
    gGame.SetUpdateCallback(MainLoop);
    
    gGame.RunLoop();

    return 0;
}