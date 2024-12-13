#include "game.hpp"

Game::Game() : mOpenGLContext(nullptr), mWindow(nullptr), mScreenWidth(640) , mScreenHeight(480), r(0.0510f), g(0.1255f), b(0.1490f) {

}
Game::~Game() {
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;

    SDL_Quit();
}

void Game::InitializeProgram(std::string name, int x, int y, int w, int h) {

    mScreenWidth = w;
    mScreenHeight = h;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initialize window" << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    mWindow = SDL_CreateWindow(name.c_str(), x, y, mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL);

    if(mWindow == nullptr) {
        std::cerr << "SDL window was not able to initialize" << std::endl;
        exit(1);
    }

    mOpenGLContext = SDL_GL_CreateContext(mWindow);

    if(mOpenGLContext == nullptr) {
        std::cerr << "SDL opengl context was not able to initialize" << std::endl;
        exit(1);
    }

    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cerr << "Glad was not initialized" << std::endl;
        exit(1);
    }

    utility::GetOpenGlVersionInfo();
}

void Game::SetBackgroundColor(uint8_t _r, uint8_t _g, uint8_t _b) {
    r = (float)_r/255;
    g = (float)_g/255;
    b = (float)_b/255;
}
void Game::SetEventCallback(std::function<void(void)> func) {
    mEventCallback = func;
}
void Game::SetUpdateCallback(std::function<void(void)> func) {
    mUpdateCallback = func;
}


void Game::RunLoop() {

        while (!mQuit) {
        mEventCallback();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glViewport(0,0, mScreenWidth, mScreenHeight);
        glClearColor(r, g, b, 1.0f);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        mUpdateCallback();

        SDL_GL_SwapWindow(mWindow);
    }
    
}

void Game::StopLoop() {
    mQuit = true;
}