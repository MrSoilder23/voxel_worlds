// C++ standard libraries
#include <limits>
#include <iostream>
#include <random>

#include <SDL2/SDL.h>

// Own libraries
#include "./utility/open_simplex_noise2.hpp"

SDL_Window* window;
SDL_Renderer* renderer;

void WindowInit() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return;
    }

    window = SDL_CreateWindow("Pixel Drawing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
    if (!window) {
        exit(1);
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        exit(1);
        return;
    }
}

void WindowExit() {
    SDL_Delay(3000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
}

int main(int argc, char* argv[]) {
    float perlinMin = std::numeric_limits<float>::infinity();
    float perlinMax = -std::numeric_limits<float>::infinity();

    static std::random_device rndDevice;
    unsigned int seed = rndDevice();
    
    float PerlinSize = 720;

    WindowInit();

    for (float x = 0; x < PerlinSize; x++) {
        for (float y = 0; y < PerlinSize; y++) {
            float perlinHeight = open_simplex_noise::LayeredNoise2D(
                x*0.01,
                y*0.01,
                seed
            );

            perlinMin = std::min(perlinMin, perlinHeight);
            perlinMax = std::max(perlinMax, perlinHeight);

            Uint8 color = static_cast<Uint8>((1.0f - perlinHeight) * 127.5f);

            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            SDL_RenderDrawPoint(renderer, x, y);

        }
    }

    SDL_RenderPresent(renderer);

    std::cout << "Min: " << perlinMin << "\nMax:  " << perlinMax << std::endl;

    WindowExit();
    return 0;
}