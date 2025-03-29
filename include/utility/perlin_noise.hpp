#pragma once
// C++ standard libraries
#include <cmath>

// Third_party libraries
#include <glm/glm.hpp>

// Own libraries
#include "./utility/constant.hpp"
#include "./utility/utility.hpp"

namespace perlin_noise {
        glm::vec2 Gradient(int x, int y, unsigned int seed);
    
        float Noise2DNormalized(int chunkX, int chunkY,float x, float y, unsigned int seed);
    
        float Noise2D(int chunkX, int chunkY,float x, float y, unsigned int seed);
    
        // Persistance - More Pronouced peaks and valleys
        // Lacunarity -  More detailed terrain
        float LayeredNoise2DNormalized(int chunkX, int chunkY, float x, float y, unsigned int seed, int octaves = 4, float persistence = 0.5f, float lacunarity = 2.0f);
        float LayeredNoise2D(int chunkX, int chunkY, float x, float y, unsigned int seed, int octaves = 4, float persistence = 0.5f, float lacunarity = 2.0f);
};      