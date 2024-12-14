// C++ Standard libraries
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Third_party libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>

// Own libraries
#include "utility.hpp"
#include "shader.hpp"
#include "game.hpp"
#include "model.hpp"
#include "model_manager.hpp"
#include "transform.hpp"

struct Settings {
    GLuint mGraphicsShaderProgram = 0;

    int mScreenWidth = 640;
    int mScreenHeight = 480;

};

struct Mesh {
    GLuint mVertexArrayObject = 0;
    GLuint mVertexBufferObject = 0;
    GLuint mVertexBufferObjectColor = 0;

    GLuint mIndexBufferObject = 0;
    GLuint mPipeline = 0;

    Transform mTransform;
};

ModelManager& gModelManager = ModelManager::GetInstance();
std::shared_ptr<Model> gBlock = std::make_shared<Model>();

Settings gSettings;
Game gGame;

Mesh mesh1;

void InitializeModels() {
    gBlock->vertexPositions = {
        0.0f, 0.0f, 0.0f, // BotLeftVertex
        1.0f, 0.0f, 0.0f, // BotRightVertex
        0.0f, 1.0f, 0.0f, // TopLeftVertex
        1.0f, 1.0f, 0.0f, // TopRightVertex
    };
    gBlock->indexBufferData = {
        0,1,2, 3,2,1, // Front Face
    };

    gModelManager.CreateNewModel("block", gBlock);
}   

void MeshCreate() {
    const std::vector<GLfloat> vertexColors {
        1.0f,  0.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
        0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 1.0f,
    };

    glGenVertexArrays(1, &mesh1.mVertexArrayObject);
    glBindVertexArray(mesh1.mVertexArrayObject);

    glGenBuffers(1, &mesh1.mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mesh1.mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, gModelManager.GetModel("block")->vertexPositions.size()*sizeof(GLfloat), gModelManager.GetModel("block")->vertexPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &mesh1.mIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh1.mIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gModelManager.GetModel("block")->indexBufferData.size()*sizeof(GLuint), gModelManager.GetModel("block")->indexBufferData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mesh1.mVertexBufferObjectColor);
    glBindBuffer(GL_ARRAY_BUFFER, mesh1.mVertexBufferObjectColor);
    glBufferData(GL_ARRAY_BUFFER, vertexColors.size()*sizeof(GLfloat), vertexColors.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

int FindUniformLocation(GLuint pipeline, const GLchar* name) {
    GLint uniformLocation = glGetUniformLocation(pipeline,name);
    if(uniformLocation < 0) {
        std::cerr << "Could not find location of: " << name << std::endl;
        exit(1); // Change to exit failure
    }

    return uniformLocation;
}

void MeshDraw() {
    glUseProgram(mesh1.mPipeline);

    glBindVertexArray(mesh1.mVertexArrayObject);

    GLint uModelMatrixLocation = FindUniformLocation(mesh1.mPipeline, "uModelMatrix");
    glUniformMatrix4fv(uModelMatrixLocation, 1, false, &mesh1.mTransform.mModelMatrix[0][0]);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

void MeshDelete() {
    glDeleteBuffers(1, &mesh1.mVertexArrayObject);
    glDeleteBuffers(1, &mesh1.mVertexBufferObjectColor);
    glDeleteBuffers(1, &mesh1.mIndexBufferObject);
    glDeleteVertexArrays(1, &mesh1.mVertexArrayObject);
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
    MeshDraw();
}

int main(int argc, char* argv[]) {

    InitializeModels();

    gGame.InitializeProgram("Giera", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gSettings.mScreenWidth, gSettings.mScreenHeight);

    shader::CreateGraphicsPipeline(gSettings.mGraphicsShaderProgram, "./shaders/vert.glsl", "./shaders/frag.glsl");
    
    mesh1.mPipeline = gSettings.mGraphicsShaderProgram;

    MeshCreate();

    gGame.SetEventCallback(Input);
    gGame.SetUpdateCallback(MainLoop);
    
    gGame.RunLoop();

    MeshDelete();

    return 0;
}