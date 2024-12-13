#pragma once
// C++ standard libraries
#include <functional>
#include <string>

// Third_party libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>

// Own libraries
#include "utility.hpp"

class Game {
    public:
        Game();
        ~Game();

        void InitializeProgram(std::string name, int x, int y, int w, int h);
        
        void SetBackgroundColor(uint8_t _r, uint8_t _g, uint8_t _b);
        void SetEventCallback(std::function<void(void)> func);
        void SetUpdateCallback(std::function<void(void)> func);

        void RunLoop();

        void StopLoop();

    private:
        std::function<void(void)> mEventCallback;
        std::function<void(void)> mUpdateCallback;
    
        SDL_GLContext mOpenGLContext;
        SDL_Window* mWindow;

        int mScreenWidth;
        int mScreenHeight;

        float r,g,b;

        bool mQuit;
};