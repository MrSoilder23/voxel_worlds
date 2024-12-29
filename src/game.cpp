#include "game.hpp"

Game::Game() :  mOpenGLContext(nullptr), mWindow(nullptr), mScreenWidth(640), mScreenHeight(480), 
                r(0.0510f),g(0.1255f),b(0.1490f), 
                mEventCallback([](float deltaTime){}), mUpdateCallback([](float deltaTime){}),
                lastTime(0) {

}
Game::~Game() {
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;

    IMG_Quit();
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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

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

    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
        printf("Failed to initialize SDL_image: %s\n", IMG_GetError());
        exit(1);
    }

    utility::GetOpenGlVersionInfo();
}

void Game::SetBackgroundColor(uint8_t _r, uint8_t _g, uint8_t _b) {
    r = (float)_r/255;
    g = (float)_g/255;
    b = (float)_b/255;
}
void Game::SetEventCallback(const std::function<void(float)>& func) {
    mEventCallback = func;
}
void Game::SetUpdateCallback(const std::function<void(float)>& func) {
    mUpdateCallback = func;
}

void Game::TurnDebug() {
    mDebug = true;
}

void GLAPIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cerr << "OpenGL Debug Message: " << message << std::endl;
}

void Game::RunLoop() {
    SDL_WarpMouseInWindow(mWindow, mScreenWidth/2, mScreenHeight/2);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!mQuit) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        mEventCallback(deltaTime);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        if(mDebug) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLDebugCallback, nullptr);
        }

        glViewport(0,0, mScreenWidth, mScreenHeight);
        glClearColor(r, g, b, 1.0f);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        mUpdateCallback(deltaTime);

        SDL_GL_SwapWindow(mWindow);
    }
    
}

void Game::StopLoop() {
    mQuit = true;
}