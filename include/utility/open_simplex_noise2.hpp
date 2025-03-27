#pragma once
// C++ standard libraries
#include <vector>

// Third_party libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace open_simplex_noise {
    static const long long HASH_MULTIPLIER = 0x53A3F72DEEC546F5LL;
    static const double NORMALIZER_2D =      0.01001634121365712;

    static constexpr long long PRIME_X = 0x5205402B9270C86FLL;
    static constexpr long long PRIME_Y = 0x598CD327003817B5LL;

    static constexpr double SKEW_2D =    0.366025403784439;
    static constexpr double UNSKEW_2D = -0.21132486540518713;

    static constexpr float RSQUARED_2D = 0.5f;

    static const int N_GRADS_2D_EXPONENT = 7;
    static const int N_GRADS_2D = 1 << N_GRADS_2D_EXPONENT;

    float Gradient(long long x, long long y, float dx, float dy, unsigned int seed);

    float Noise2D(int x, int y, unsigned int seed);
    float Noise2DUnskewedBase(double x, double y, unsigned int seed);
    float LayeredNoise2D(double x, double y, unsigned int seed, int octaves = 4, float persistence = 0.5f, float lacunarity = 2.0f);
}
