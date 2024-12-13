// C++ Standard libraries
#include <iostream>
#include <string>

// Third_party libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>

// Own libraries
#include "utility.hpp"
#include "shader.hpp"
#include "game.hpp"

struct Settings {
    GLuint mGraphicsShaderProgram = 0;

    int mScreenWidth = 640;
    int mScreenHeight = 480;

};

Settings gSettings;
Game gGame;

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

}

int main(int argc, char* argv[]) {

    gGame.InitializeProgram("Giera", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gSettings.mScreenWidth, gSettings.mScreenHeight);

    shader::CreateGraphicsPipeline(gSettings.mGraphicsShaderProgram, "./shaders/vert.glsl", "./shaders/frag.glsl");

    gGame.SetEventCallback(Input);
    gGame.SetUpdateCallback(MainLoop);
    
    gGame.RunLoop();

    return 0;
}