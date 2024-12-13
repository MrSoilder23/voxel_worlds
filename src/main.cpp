// C++ Standard libraries
#include <iostream>

// Third_party libraries
#include <SDL2/SDL.h>

SDL_GLContext gOpenGLContext = nullptr;
SDL_Window* gWindow = nullptr;

int gScreenWidth = 640;
int gScreenHeight = 580;

bool gQuit = false;


void InitializeProgram() {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initialize window" << std::endl;
        exit(1);
    }

    gWindow = SDL_CreateWindow("OpenGL project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);

    if(gWindow == nullptr) {
        std::cerr << "SDL window was not able to initialize" << std::endl;
        exit(1);
    }

    gOpenGLContext = SDL_GL_CreateContext(gWindow);

    if(gOpenGLContext == nullptr) {
        std::cerr << "SDL opengl context was not able to initialize" << std::endl;
        exit(1);
    }

}

void Input() {

    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            gQuit = true;
        }
    }

}

void MainLoop() {

    while (!gQuit) {
        Input();

        SDL_GL_SwapWindow(gWindow);
    }
    
}

void CleanUp() {
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    SDL_Quit();
}

int main(int argc, char* argv[]) {

    InitializeProgram();

    MainLoop();

    CleanUp();

    return 0;
}