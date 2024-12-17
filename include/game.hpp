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
        void SetEventCallback(const std::function<void(float)>& func);
        void SetUpdateCallback(const std::function<void(float)>& func);

        void RunLoop();

        void StopLoop();

    private:
        std::function<void(float)> mEventCallback;
        std::function<void(float)> mUpdateCallback;
    
        SDL_GLContext mOpenGLContext;
        SDL_Window* mWindow;

        Uint32 lastTime;

        int mScreenWidth;
        int mScreenHeight;

        float r,g,b;

        bool mQuit;
};