// C++ standard libraries
#include <limits>
#include <iostream>
#include <random>
#include <array>
#include <algorithm>

#include <SDL2/SDL.h>

// Own libraries
#include "./utility/open_simplex_noise2.hpp"
#include "./utility/utility.hpp"

SDL_Window* window;
SDL_Renderer* renderer;

bool isRunning = true;

struct Spline1 {
    std::vector<std::pair<float, float>> keypoints;

    float evaluate(float x) {
        // Find the segment containing x
        auto it = std::lower_bound(keypoints.begin(), keypoints.end(), x, 
            [](const auto& a, float b) { return a.first < b; });
        
        if (it == keypoints.begin()) return keypoints.front().second;
        if (it == keypoints.end()) return keypoints.back().second;
        
        auto& [x1, y1] = *(it - 1);
        auto& [x2, y2] = *it;
        
        float t = (x - x1) / (x2 - x1);
        return y1 + t * (y2 - y1);
    }
};

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

Spline1 continentalSpline, erosionSpline, peaksValleysSpline;
void initSplines() {
    continentalSpline.keypoints = {
        {-1.0f, 400.0f},
        {-0.8f, 30.0f},
        {-0.55f, 30.0f},
        {-0.25f, 130.0f},
        {-0.2f, 130.0f},
        { 0.05f, 135.0f},
        { 0.45f, 160.0f},
        { 1.0f, 300.0f}};

    erosionSpline.keypoints = {
        {-1.0f, 500.0f},
        {-0.45f,300.0f},
        {-0.4f, 250.0f},
        {-0.15f,270.0f},
        { 0.2f, 70.0f},
        { 0.4f, 60.0f},
        { 0.7f, 135.0f},
        { 1.0f, 60.0f}};

    peaksValleysSpline.keypoints = {
        {-1.0f, 100.0f},
        {-0.6f, 50.0f},
        {-0.1f, -100.0f},
        { 0.0f, -100.0f},
        { 0.2f, 50.0f},
        { 0.4f, 80.0f},
        { 0.7f, 85.0f},
        { 1.0f, 100.0f}};
}

float GetHeightMultiplier(float continentalness, std::array<std::pair<float, float>, 8> keyPoints) {    
    for (size_t i = 0; i < keyPoints.size() - 1; ++i) {
        if (continentalness >= keyPoints[i].first && continentalness <= keyPoints[i + 1].first) {
            float interpolation = (continentalness - keyPoints[i].first) / (keyPoints[i + 1].first - keyPoints[i].first);

            float smoothedInterpolation = utility::Smooth(interpolation);

            // lerp
            return glm::mix(keyPoints[i].second, keyPoints[i + 1].second, smoothedInterpolation);
        }
    }

    // If continentalness is outside the defined range, return the closest multiplier
    return continentalness < keyPoints.front().first ? keyPoints.front().second : keyPoints.back().second;
}

int main(int argc, char* argv[]) {
    float noiseMin = std::numeric_limits<float>::infinity();
    float noiseMax = -std::numeric_limits<float>::infinity();

    static std::random_device rndDevice;
    unsigned int seed = rndDevice();
    
    float noiseSize = 400;

    WindowInit();
    initSplines();

    for (float x = -noiseSize; x < noiseSize; x++) {
        for (float y = -noiseSize; y < noiseSize; y++) {
            float continentalness = open_simplex_noise::LayeredNoise2D(
                x * (0.005f / 2),
                y * (0.005f / 2),
                seed,
                4,
                0.5f,
                2.5f
            );
            float erosion = open_simplex_noise::LayeredNoise2D(
                y * (0.004f / 2),
                x * (0.004f / 2),
                seed,
                4,
                0.4f,
                2.4f
            );
            float peaksAndValleys = open_simplex_noise::LayeredNoise2D(
                y * (0.025f / 6),
                x * (0.025f / 6),
                seed,
                6,
                0.4f,
                1.8f
            );

            float continentalAdj = continentalSpline.evaluate(continentalness);
            float erosionAdj = erosionSpline.evaluate(erosion);
            float peaksValleysAdj = peaksValleysSpline.evaluate(peaksAndValleys);

            float noiseHeight = (continentalAdj * 0.5f) + (erosionAdj * 0.3f) + (peaksValleysAdj * 0.2f);

              
            noiseMin = std::min(noiseMin, noiseHeight);
            noiseMax = std::max(noiseMax, noiseHeight);

            Uint8 color = static_cast<Uint8>((1.0f - (noiseHeight / 250.0f)) * 255.0f);
            

            // Continental
            // if(noiseHeight <= -0.55f) {
            //     SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);
            // } else if(noiseHeight > -0.55f && noiseHeight <= -0.25f) {
            //     SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
            // } else if(noiseHeight > -0.25f && noiseHeight <= -0.2f) {
            //     SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
            // } else if(noiseHeight > -0.2f && noiseHeight <= 0.05f) {
            //     SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
            // } else if(noiseHeight > 0.05f && noiseHeight <= 0.45f) {
            //     SDL_SetRenderDrawColor(renderer, 228, 228, 228, 255);
            // } else if(noiseHeight > 0.45f && noiseHeight <= 1.0f) {
            //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  
            // } else {
            //     SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            // }

            // Erosion
            // if(noiseHeight <= -0.45f) {
            //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  
            // } else if(noiseHeight > -0.45f && noiseHeight <= -0.4f) {
            //     SDL_SetRenderDrawColor(renderer, 228, 228, 228, 255);
            // } else if(noiseHeight > -0.4f && noiseHeight <= -0.15f) {
            //     SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
            // } else if(noiseHeight > -0.15f && noiseHeight <= 0.2f) {
            //     SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
            // } else if(noiseHeight > 0.2f && noiseHeight <= 0.4f) {
            //     SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
            // } else if(noiseHeight > 0.4f && noiseHeight <= 0.7f) {
            //     SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);
            // } else if(noiseHeight > 0.7f && noiseHeight <= 1.0f) {
            //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            // } else {
            //     SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            // }

            // Peaks and valleys
            // if(noiseHeight <= -0.6f) {
            //     SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);  
            // } else if(noiseHeight > -0.6f && noiseHeight <= -0.1f) {
            //     SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);  
            // } else if(noiseHeight > -0.1f && noiseHeight <= 0.1f) {
            //     SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);  
            // } else if(noiseHeight > 0.1f && noiseHeight <= 0.2f) {
            //     SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);  
            // } else if(noiseHeight > 0.2f && noiseHeight <= 0.4f) {
            //     SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);  
            // } else if(noiseHeight > 0.4f && noiseHeight <= 0.7f) {
            //     SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);  
            // } else if(noiseHeight > 0.7f && noiseHeight <= 1.0f) {
            //     SDL_SetRenderDrawColor(renderer, 228, 228, 228, 255);  
            // } else {
            //     SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            // }

            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            SDL_RenderDrawPoint(renderer, (x+noiseSize), (y+noiseSize));

        }
    }

    SDL_RenderPresent(renderer);

    std::cout << "Min: " << noiseMin << "\nMax:  " << noiseMax << std::endl;

    WindowExit();
    return 0;
}