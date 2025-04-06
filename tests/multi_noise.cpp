// C++ standard libraries
#include <limits>
#include <iostream>
#include <random>

#include <SDL2/SDL.h>

// Own libraries
#include "./utility/open_simplex_noise2.hpp"

SDL_Window* window;
SDL_Renderer* renderer;

bool isRunning = true;

void WindowInit() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return;
    }

    window = SDL_CreateWindow("MultiNoise", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
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
    while (isRunning) {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                isRunning = false;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
}

int main(int argc, char* argv[]) {
    float noiseMin = std::numeric_limits<float>::infinity();
    float noiseMax = -std::numeric_limits<float>::infinity();

    static std::random_device rndDevice;
    unsigned int seed = rndDevice();
    
    float noiseSize = 800;

    WindowInit();

    for (float x = 0; x < noiseSize; x++) {
        for (float y = 0; y < noiseSize; y++) {
            float continentalness = open_simplex_noise::LayeredNoise2D(
                x * (0.005f / 2),
                y * (0.005f / 2),
                seed,
                4,
                0.5f,
                2.5f
            );
            float erosion = open_simplex_noise::LayeredNoise2D(
                x * (0.003f / 2),
                y * (0.003f / 2),
                seed,
                4,
                0.5f,
                2.0f
            );
            float peaksAndValleys = open_simplex_noise::LayeredNoise2D(
                x * (0.03f / 2),
                y * (0.03f / 2),
                seed,
                7,
                0.5f,
                2.0f
            );

            float noiseHeight = ((continentalness * 2.0f) + (erosion * -1.5f) + (peaksAndValleys * 0.7f));

            noiseMin = std::min(noiseMin, noiseHeight);
            noiseMax = std::max(noiseMax, noiseHeight);

            Uint8 color = static_cast<Uint8>((1.0f - noiseHeight) * 127.5f);

            if(noiseHeight <= -0.2f) {
                SDL_SetRenderDrawColor(renderer, 71, 92, 244, 255);  
            } else if(noiseHeight > -0.2f && noiseHeight <= 0.0f) {
                SDL_SetRenderDrawColor(renderer, 78, 149, 230, 255);
            } else if(noiseHeight > 0.0f && noiseHeight <= 0.1f) {
                SDL_SetRenderDrawColor(renderer, 245, 227, 149, 255);
            } else if(noiseHeight > 0.1f && noiseHeight <= 0.4f) {
                SDL_SetRenderDrawColor(renderer, 118, 209, 90, 255);
            } else if(noiseHeight > 0.4f && noiseHeight <= 0.9f) {
                SDL_SetRenderDrawColor(renderer, 67, 191, 77, 255);
            } else if(noiseHeight > 0.9f && noiseHeight <= 1.0f) {
                SDL_SetRenderDrawColor(renderer, 228, 244, 253, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            }

            SDL_RenderDrawPoint(renderer, x, y);

        }
    }

    SDL_RenderPresent(renderer);

    std::cout << "Min: " << noiseMin << "\nMax:  " << noiseMax << std::endl;

    WindowExit();
    return 0;
}