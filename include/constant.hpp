#pragma once

namespace VoxelWorlds {
    const int RENDER_DISTANCE = 6;
    const float CHUNK_SIZE = 32;
    const int CHUNK_GENERATION_OFFSET = 5;
    const int PERLIN_SCALE = 4;

    // Perlin noise settings
    const int   OCTAVES =     6;
    const float PERSISTANCE = 0.3f;
    const float LACUNARITY =  2.7f;
    
    const size_t THREAD_AMOUNT = 4;
}