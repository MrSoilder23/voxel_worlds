// C++ standard libraries
#include <limits>
#include <iostream>
#include <random>

#include <SDL2/SDL.h>

// Own libraries
#include "./utility/open_simplex_noise2.hpp"

int main(int argc, char* argv[]) {
    float perlinMin = std::numeric_limits<float>::infinity();
    float perlinMax = -std::numeric_limits<float>::infinity();

    static std::random_device rndDevice;
    unsigned int seed = rndDevice();
    
    float PerlinSize = 720;

    for (float x = 0; x < PerlinSize; x++) {
        for (float y = 0; y < PerlinSize; y++) {
            float perlinHeight = open_simplex_noise::Noise2D(
                (int)x,
                (int)y,
                seed
            );

            perlinMin = std::min(perlinMin, perlinHeight);
            perlinMax = std::max(perlinMax, perlinHeight);
        }
    }

    std::cout << "Min: " << perlinMin << "\nMax:  " << perlinMax << std::endl;

    return 0;
}