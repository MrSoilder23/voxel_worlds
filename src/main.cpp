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

struct Settings {
    Camera mCamera;

    GLuint mGraphicsShaderProgram = 0;

    int mScreenWidth = 640;
    int mScreenHeight = 480;

};

ModelManager& gModelManager = ModelManager::GetInstance();
std::shared_ptr<Model> gBlock = std::make_shared<Model>();
std::shared_ptr<MeshData> gMesh1 = std::make_shared<MeshData>(); 

Settings gSettings;
Game gGame;

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

void MeshCreate(std::shared_ptr<MeshData> mesh, std::shared_ptr<Model> model) {
    /*
    const std::vector<GLfloat> vertexColors {
         1.0f,  0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,

         1.0f,  0.0f, 0.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  1.0f, 0.0f,
         1.0f,  0.0f, 0.0f,
    };

    glGenVertexArrays(1, &mesh->mVertexArrayObject);
    glBindVertexArray(mesh->mVertexArrayObject);

    glGenBuffers(1, &mesh->mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, model->vertexPositions.size()*sizeof(GLfloat), model->vertexPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &mesh->mIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indexBufferData.size()*sizeof(GLuint), model->indexBufferData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mesh->mVertexBufferObjectColor);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->mVertexBufferObjectColor);
    glBufferData(GL_ARRAY_BUFFER, vertexColors.size()*sizeof(GLfloat), vertexColors.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    */
}

void MeshDraw(std::shared_ptr<MeshData> mesh) {
    /*
    glUseProgram(mesh->mPipeline);

    glBindVertexArray(mesh->mVertexArrayObject);

    GLint uModelMatrixLocation = shader::FindUniformLocation(mesh->mPipeline, "uModelMatrix");
    glUniformMatrix4fv(uModelMatrixLocation, 1, false, &mesh->mTransform.mModelMatrix[0][0]);

    glm::mat4 view = gSettings.mCamera.GetViewMatrix();
    GLint uViewLocation = shader::FindUniformLocation(mesh->mPipeline, "uViewMatrix");
    glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);

    glm::mat4 perspective = gSettings.mCamera.GetProjectionMatrix();
    GLint uProjectionLocation = shader::FindUniformLocation(mesh->mPipeline, "uProjectionMatrix");
    glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
    */
}

void MeshDelete(std::shared_ptr<MeshData> mesh) {

}

void Input() {

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

void MainLoop() {
    MeshDraw(gMesh1);
}

int main(int argc, char* argv[]) {

    InitializeModels();

    gSettings.mCamera.SetProjectionMatrix(glm::radians(45.0f), (float)gSettings.mScreenWidth/(float)gSettings.mScreenHeight, 0.1f);
    gGame.InitializeProgram("Giera", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gSettings.mScreenWidth, gSettings.mScreenHeight);

    shader::CreateGraphicsPipeline(gSettings.mGraphicsShaderProgram, "./shaders/vert.glsl", "./shaders/frag.glsl");
    
    gMesh1->mPipeline = gSettings.mGraphicsShaderProgram;

    MeshCreate(gMesh1, gModelManager.GetModel("block"));
    utility::MeshTranslate(gMesh1, 0.0f, 0.0f, -10.0f);
    utility::MeshRotate(gMesh1, 45, glm::vec3(0.0f, 1.0f, 0.0f));

    gGame.SetEventCallback(Input);
    gGame.SetUpdateCallback(MainLoop);
    
    gGame.RunLoop();

    MeshDelete(gMesh1);

    return 0;
}