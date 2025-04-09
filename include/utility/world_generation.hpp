#pragma once
// Own libraries
#include "./utility/open_simplex_noise2.hpp"
#include "./utility/constant.hpp"

namespace world_generation {
    float GenerateHeight(const unsigned int& seed, int x, int z);
}