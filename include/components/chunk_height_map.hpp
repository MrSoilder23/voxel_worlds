#pragma once
// C++ standard libraries
#include <array>

// Own libraries
#include "utility/constant.hpp"

struct ChunkHeightMapComponent {
    std::array<float, VoxelWorlds::CHUNK_SIZE_2D> heightMap;
};