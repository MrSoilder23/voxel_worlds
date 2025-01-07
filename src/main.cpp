// C++ Standard libraries
#include <iostream>
#include <string>
#include <memory>
#include <random>

// Third_party libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

// Own libraries
#include "shader.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "renderer_system.hpp"
#include "blocks.hpp"
#include "chunk_manager.hpp"
#include "chunk_renderer_system.hpp"
#include "chunk_system.hpp"

struct Settings {
    GLuint mGraphicsShaderProgram = 0;

    int mScreenWidth = 640;
    int mScreenHeight = 480;

    float mSpeed = 20.0f;
    float mSensitivity = 10.0f;
};
Settings gSettings;
Game gGame;

std::shared_ptr<GraphicsApp> gGraphicsApp = std::make_shared<GraphicsApp>();
EntityManager& gEntityManager = EntityManager::GetInstance();

RendererSystem& gRendererSystem = RendererSystem::GetInstance();
ChunkRendererSystem& gChunkRendererSystem = ChunkRendererSystem::GetInstance();
ChunkManager& gChunkManager = ChunkManager::GetInstance();

ChunkSystem chunkSystem;

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

    // auto chunk = gChunkManager.GetChunk(0,0,0);
    // gChunkRendererSystem.DrawChunk(chunk);
    chunkSystem.SetCameraPosition(gGraphicsApp->mCamera.GetEye());
    chunkSystem.GenerateWorld();
    chunkSystem.DrawChunks();
}

int main(int argc, char* argv[]) {

    gGame.InitializeProgram("Giera", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gSettings.mScreenWidth, gSettings.mScreenHeight);
    gGraphicsApp->mCamera.SetProjectionMatrix(glm::radians(45.0f), (float)gSettings.mScreenWidth/(float)gSettings.mScreenHeight, 0.1f);

    shader::CreateGraphicsPipeline(gSettings.mGraphicsShaderProgram, "./shaders/vert.glsl", "./shaders/frag.glsl");
    gGraphicsApp->mGraphicsPipeline = gSettings.mGraphicsShaderProgram;

    InitializeModels();
    InitializeTextures();
    InitializeBlocks();

    static std::random_device rndDevice;
    unsigned int seed = rndDevice();

    chunkSystem.SetWorldSeed(seed);
    chunkSystem.SetRenderDistance(6.0);

    // auto chunk = gChunkManager.CreateChunk(0,0,0);
    // auto grass_block = gEntityManager.GetEntity("grass_block");
    // auto dirt_block = gEntityManager.GetEntity("dirt_block");
    // for(float i = 0; i < 32; i++) {
    //     for(float j = 0; j < 32; j++) {
    //         float height = std::round(PerlinNoise(0,0 ,i/32.0, j/32.0, seed) * 32.0);
    //         for(float k = 0; k < height; k++) {
    //             if(k == height-1) {
    //                 gChunkManager.InsertToChunk(chunk, grass_block, i, k, j);
    //             } else {
    //                 gChunkManager.InsertToChunk(chunk, dirt_block, i, k, j);
                    
    //             }
            
    //         }
    //     }
    // }


    gChunkManager.InitializeChunk(0,0,0);

    // gRendererSystem.AddGraphicsApp(gGraphicsApp);
    gChunkRendererSystem.AddGraphicsApp(gGraphicsApp);

    gGame.SetEventCallback(Input);
    gGame.SetUpdateCallback(MainLoop);
    
    gGame.RunLoop();

    return 0;
}