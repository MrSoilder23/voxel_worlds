#pragma once
// C++ standard libraries
#include <array>

// Own libraries
#include "./utility/spline.hpp"

namespace VoxelWorlds {
    static constexpr int   CHUNK_GENERATION_OFFSET = 12;
    static constexpr int   RENDER_DISTANCE = 8;
    static constexpr float CHUNK_SIZE =      32;
    
    // Noise paint settings
    static constexpr int PERLIN_SCALE =  4;

    static constexpr int   SCALE =       3;
    static constexpr int   OCTAVES =     6;
    static constexpr float PERSISTANCE = 0.3f;
    static constexpr float LACUNARITY =  2.7f;

    static Spline CONTINENTAL_SPLINE({
        {-1.0f, 400.0f},
        {-0.8f, 30.0f},
        {-0.55f, 30.0f},
        {-0.25f, 130.0f},
        {-0.2f, 130.0f},
        { 0.05f, 135.0f},
        { 0.45f, 250.0f},
        { 1.0f, 500.0f}
    });
    static Spline EROSION_SPLINE({
        {-1.0f, 400.0f},
        {-0.8f, 30.0f},
        {-0.55f, 30.0f},
        {-0.25f, 130.0f},
        {-0.2f, 130.0f},
        { 0.05f, 135.0f},
        { 0.45f, 250.0f},
        { 1.0f, 500.0f}
    });
    static Spline PEAKS_VALLEYS_SPLINE({
        {-1.0f, 100.0f},
        {-0.6f, 50.0f},
        {-0.1f, -100.0f},
        { 0.0f, -100.0f},
        { 0.2f, 50.0f},
        { 0.4f, 80.0f},
        { 0.7f, 85.0f},
        { 1.0f, 100.0f}
    });

    // Game default settings
    static constexpr size_t THREAD_AMOUNT = 6;
    static constexpr float FRAME_RATE = 240;
}