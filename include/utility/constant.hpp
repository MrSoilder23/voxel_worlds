#pragma once

namespace VoxelWorlds {
    constexpr int RENDER_DISTANCE = 6;
    constexpr float CHUNK_SIZE = 32;
    constexpr int CHUNK_GENERATION_OFFSET = 5;
    constexpr int PERLIN_SCALE = 4;

    // Perlin noise settings
    constexpr int   OCTAVES =     6;
    constexpr float PERSISTANCE = 0.3f;
    constexpr float LACUNARITY =  2.7f;
    
    constexpr size_t THREAD_AMOUNT = 4;
    constexpr float FRAME_RATE = 240;
}