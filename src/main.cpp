// C++ Standard libraries
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Third_party libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Own libraries
#include "utility.hpp"
#include "shader.hpp"
#include "game.hpp"
#include "model.hpp"
#include "model_manager.hpp"
#include "transform.hpp"
#include "camera.hpp"
#include "mesh_data.hpp"
#include "graphics.hpp"
#include "model_component.hpp"
#include "entity_manager.hpp"
#include "renderer_system.hpp"

struct Settings {
    GLuint mGraphicsShaderProgram = 0;

    int mScreenWidth = 640;
    int mScreenHeight = 480;

};

ModelManager& gModelManager = ModelManager::GetInstance();
std::shared_ptr<Model> gBlock = std::make_shared<Model>();
std::shared_ptr<MeshData> gMesh1 = std::make_shared<MeshData>(); 

EntityManager& gEntityManager = EntityManager::GetInstance();
RendererSystem& gRendererSystem = RendererSystem::GetInstance();

Settings gSettings;
Game gGame;
std::shared_ptr<GraphicsApp> gGraphicsApp = std::make_shared<GraphicsApp>();

void InitializeModels() {
    
    gBlock->vertexPositions = {
        -0.5f, -0.5f,  0.5f, // BotLeftVertex
         0.5f, -0.5f,  0.5f, // BotRightVertex
        -0.5f,  0.5f,  0.5f, // TopLeftVertex
         0.5f,  0.5f,  0.5f, // TopRightVertex
    
        -0.5f, -0.5f, -0.5f, // BackBotLeft
         0.5f, -0.5f, -0.5f, // BackBotRight
        -0.5f,  0.5f, -0.5f, // BackTopLeft
         0.5f,  0.5f, -0.5f, // BackTopRight
    };
    gBlock->indexBufferData = {
        0,1,2, 3,2,1, // FrontFace
        0,4,5, 1,0,5, // BottomFace
        6,7,4, 7,5,4, // BackFace
        6,2,7, 3,7,2, // TopFace
        3,1,7, 5,7,1, // RightFace
        6,4,2, 0,2,4, // LeftFace
    };

    gModelManager.CreateNewModel("block", gBlock);
    
}   

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
    if(state[SDL_SCANCODE_W]) {
        gGraphicsApp->mCamera.MoveForward(1.0f, deltaTime);
    }
    if(state[SDL_SCANCODE_S]) {
        gGraphicsApp->mCamera.MoveBackward(1.0f, deltaTime);
    }
    if(state[SDL_SCANCODE_A]) {
        gGraphicsApp->mCamera.MoveLeft(1.0f, deltaTime);
    }
    if(state[SDL_SCANCODE_D]) {
        gGraphicsApp->mCamera.MoveRight(10.0f, deltaTime);
    }


}

void MainLoop(float deltaTime) {
    gRendererSystem.DrawAll();
}

int main(int argc, char* argv[]) {

    InitializeModels();

    gGame.InitializeProgram("Giera", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gSettings.mScreenWidth, gSettings.mScreenHeight);
    gGraphicsApp->mCamera.SetProjectionMatrix(glm::radians(45.0f), (float)gSettings.mScreenWidth/(float)gSettings.mScreenHeight, 0.1f);

    shader::CreateGraphicsPipeline(gSettings.mGraphicsShaderProgram, "./shaders/vert.glsl", "./shaders/frag.glsl");
    gGraphicsApp->mGraphicsPipeline = gSettings.mGraphicsShaderProgram;
    
    // MeshCreate(gMesh1, gModelManager.GetModel("block"));
    // utility::MeshTranslate(gMesh1, 0.0f, 0.0f, -10.0f);
    // utility::MeshRotate(gMesh1, 45, glm::vec3(0.0f, 1.0f, 0.0f));

    int block = gEntityManager.CreateEntity();
    int block1 = gEntityManager.CreateEntity();
    std::cout << "Block: " << block << std::endl;
    gEntityManager.AddComponent<ModelComponent>(block);
    gEntityManager.AddComponent<ModelComponent>(block1);
    ModelComponent& blockData = *gEntityManager.GetComponent<ModelComponent>(block);
    ModelComponent& blockData1 = *gEntityManager.GetComponent<ModelComponent>(block1);

    blockData.AddModel(gModelManager.GetModel("block"));
    utility::MeshTranslate(blockData.GetMeshData(), 2.0f, 0.0f, -10.0f);
    utility::MeshRotate(blockData.GetMeshData(), 45, glm::vec3(0.0f, 1.0f, 0.0f));
    
    blockData1.AddModel(gModelManager.GetModel("block"));
    utility::MeshTranslate(blockData1.GetMeshData(), 0.0f, 0.0f, -10.0f);
    utility::MeshRotate(blockData1.GetMeshData(), 45, glm::vec3(1.0f, 0.0f, 0.0f));

    gEntityManager.InitializeAllComponents();
    gRendererSystem.AddGraphicsApp(gGraphicsApp);

    gGame.SetEventCallback(Input);
    gGame.SetUpdateCallback(MainLoop);
    
    gGame.RunLoop();

    return 0;
}