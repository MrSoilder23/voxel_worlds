// C++ Standard libraries
#include <iostream>
#include <string>

// Third_party libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>

// Own libraries
#include "utility.hpp"
#include "shader.hpp"

struct App {
    SDL_GLContext mOpenGLContext = nullptr;
    SDL_Window* mWindow = nullptr;

    GLuint mGraphicsShaderProgram = 0;

    int mScreenWidth = 640;
    int mScreenHeight = 480;

    bool mQuit = false;
};

App gApp;

void InitializeProgram() {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initialize window" << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    gApp.mWindow = SDL_CreateWindow("OpenGL project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gApp.mScreenWidth, gApp.mScreenHeight, SDL_WINDOW_OPENGL);

    if(gApp.mWindow == nullptr) {
        std::cerr << "SDL window was not able to initialize" << std::endl;
        exit(1);
    }

    gApp.mOpenGLContext = SDL_GL_CreateContext(gApp.mWindow);

    if(gApp.mOpenGLContext == nullptr) {
        std::cerr << "SDL opengl context was not able to initialize" << std::endl;
        exit(1);
    }

    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cerr << "Glad was not initialized" << std::endl;
        exit(1);
    }

    utility::GetOpenGlVersionInfo();

}

void Input() {

    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            gApp.mQuit = true;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]) {
        gApp.mQuit = true;
    }

}

void MainLoop() {

    while (!gApp.mQuit) {
        Input();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glViewport(0,0, gApp.mScreenWidth, gApp.mScreenHeight);
        glClearColor(0.0510f, 0.1255f, 0.1490f, 1.0f);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(gApp.mWindow);
    }
    
}

void CleanUp() {
    SDL_DestroyWindow(gApp.mWindow);
    gApp.mWindow = nullptr;

    SDL_Quit();
}

int main(int argc, char* argv[]) {

    InitializeProgram();

    shader::CreateGraphicsPipeline(gApp.mGraphicsShaderProgram, "./shaders/vert.glsl", "./shaders/frag.glsl");

    MainLoop();

    CleanUp();

    return 0;
}