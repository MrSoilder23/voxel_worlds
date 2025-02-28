#pragma once
// C++ standard libraries
#include <array>

namespace VoxelWorlds {
    static constexpr int RENDER_DISTANCE = 6;
    static constexpr float CHUNK_SIZE = 32;
    static constexpr int CHUNK_GENERATION_OFFSET = 5;
    
    // Perlin noise settings
    static constexpr int PERLIN_SCALE =    4;  // Scale of perlin noise
    static constexpr int CONTINENTALNESS = 20; // Scale of continents

    // continentalness, height multiplier
    static constexpr std::array<std::pair<float, float>, 9> CONTINENTAL_KEY_POINTS = {{
        {-1.0f, 600.0f},
        {-0.8f, 100.0f},
        {-0.4f, 150.0f},
        {-0.1f, 150.0f},
        { 0.0f, 400.0f},
        { 0.6f, 400.0f},
        { 0.8f, 500.0f},
        { 0.9f, 900.0f},
        { 1.0f, 1000.0f}
    }};
    
    static constexpr int   OCTAVES =     6;
    static constexpr float PERSISTANCE = 0.3f;
    static constexpr float LACUNARITY =  2.7f;

    // Game default settings
    static constexpr size_t THREAD_AMOUNT = 5;
    static constexpr float FRAME_RATE = 240;
}