#pragma once

namespace VoxelWorlds {
    static constexpr int RENDER_DISTANCE = 6;
    static constexpr float CHUNK_SIZE = 32;
    static constexpr int CHUNK_GENERATION_OFFSET = 5;
    static constexpr int PERLIN_SCALE = 4;

    // Perlin noise settings
    static constexpr int   OCTAVES =     6;
    static constexpr float PERSISTANCE = 0.3f;
    static constexpr float LACUNARITY =  2.7f;
    
    static constexpr size_t THREAD_AMOUNT = 2;
    static constexpr float FRAME_RATE = 240;
}