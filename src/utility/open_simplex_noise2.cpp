#include "./utility/open_simplex_noise2.hpp"

float open_simplex_noise::Gradient(long long x, long long y, float dx, float dy, unsigned int seed) {
    static const auto GRADIENTS_2D = []() {
        std::vector<float> grad2 = {
            0.38268343236509f,   0.923879532511287f,
            0.923879532511287f,  0.38268343236509f,
            0.923879532511287f, -0.38268343236509f,
            0.38268343236509f,  -0.923879532511287f,
           -0.38268343236509f,  -0.923879532511287f,
           -0.923879532511287f, -0.38268343236509f,
           -0.923879532511287f,  0.38268343236509f,
           -0.38268343236509f,   0.923879532511287f,
           //-------------------------------------//
            0.130526192220052f,  0.99144486137381f,
            0.608761429008721f,  0.793353340291235f,
            0.793353340291235f,  0.608761429008721f,
            0.99144486137381f,   0.130526192220051f,
            0.99144486137381f,  -0.130526192220051f,
            0.793353340291235f, -0.60876142900872f,
            0.608761429008721f, -0.793353340291235f,
            0.130526192220052f, -0.99144486137381f,
           -0.130526192220052f, -0.99144486137381f,
           -0.608761429008721f, -0.793353340291235f,
           -0.793353340291235f, -0.608761429008721f,
           -0.99144486137381f,  -0.130526192220052f,
           -0.99144486137381f,   0.130526192220051f,
           -0.793353340291235f,  0.608761429008721f,
           -0.608761429008721f,  0.793353340291235f,
           -0.130526192220052f,  0.99144486137381f,
       };
       for (int i = 0; i < grad2.size(); i++) {
           grad2[i] = (float)(grad2[i] / NORMALIZER_2D);
       }

       std::vector<float> gradients(N_GRADS_2D * 2);
       for (int i = 0, j = 0; i < gradients.size(); i++, j++) {
           if (j == grad2.size()) j = 0;
           gradients[i] = grad2[j];
       }

       return gradients;
    }();

    long long hash = seed ^ x ^ y;

    hash *= HASH_MULTIPLIER;
    hash ^= hash >> (64 - N_GRADS_2D_EXPONENT + 1);
    int gi = (int)hash & ((N_GRADS_2D - 1) << 1);

    return GRADIENTS_2D[gi | 0] * dx + GRADIENTS_2D[gi | 1] * dy;
}

float open_simplex_noise::Noise2D(int x, int y, unsigned int seed) {
    double s = SKEW_2D * (x + y);
    double xs = x + s, ys = y + s;

    return Noise2DUnskewedBase(xs, ys, seed);
}
float open_simplex_noise::Noise2DUnskewedBase(double x, double y, unsigned int seed) {
    // Get base points and offsets.
    int xsb = std::floor(x);
    int ysb = std::floor(y);

    float xi = static_cast<float>(x - xsb);
    float yi = static_cast<float>(y - ysb);

    // Prime pre-multiplication for hash.
    long long xsbp = xsb * PRIME_X;
    long long ysbp = ysb * PRIME_Y;

    float t = (xi + yi) * static_cast<float>(UNSKEW_2D);
    float dx0 = xi + t, dy0 = yi + t;

    // First vertex.
    float value = 0;
    float a0 = RSQUARED_2D - dx0 * dx0 - dy0 * dy0;
    if (a0 > 0) {
        value = (a0 * a0) * (a0 * a0) * Gradient(xsbp, ysbp, dx0, dy0, seed);
    }

    // Second vertex.
    float a1 = static_cast<float>(2 * (1 + 2 * UNSKEW_2D) * (1 / UNSKEW_2D + 2)) * t + (static_cast<float>(-2 * (1 + 2 * UNSKEW_2D) * (1 + 2 * UNSKEW_2D)) + a0);
    if (a1 > 0) {
        float dx1 = dx0 - static_cast<float>(1 + 2 * UNSKEW_2D);
        float dy1 = dy0 - static_cast<float>(1 + 2 * UNSKEW_2D);
        value += (a1 * a1) * (a1 * a1) * Gradient(xsbp + PRIME_X, ysbp + PRIME_Y, dx1, dy1, seed);
    }

    // Third vertex.
    if (dy0 > dx0) {
        float dx2 = dx0 - static_cast<float>(UNSKEW_2D);
        float dy2 = dy0 - static_cast<float>(UNSKEW_2D + 1);
        float a2 = RSQUARED_2D - dx2 * dx2 - dy2 * dy2;
        if (a2 > 0) {
            value += (a2 * a2) * (a2 * a2) * Gradient(xsbp, ysbp + PRIME_Y, dx2, dy2, seed);
        }
    }
    else
    {
        float dx2 = dx0 - static_cast<float>(UNSKEW_2D + 1);
        float dy2 = dy0 - static_cast<float>(UNSKEW_2D);
        float a2 = RSQUARED_2D - dx2 * dx2 - dy2 * dy2;
        if (a2 > 0) {
            value += (a2 * a2) * (a2 * a2) * Gradient(xsbp + PRIME_X, ysbp, dx2, dy2, seed);
        }
    }

    return value;
}
float open_simplex_noise::LayeredNoise2D(double x, double y, unsigned int seed, int octaves, float persistence, float lacunarity) {
    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f; 
    float maxAmplitude = 0.0f;

    for (int i = 0; i < octaves; ++i) {
        float scaledX = x * frequency;
        float scaledY = y * frequency;

        float noise = Noise2D(scaledX,scaledY, seed);
        total += noise * amplitude;

        maxAmplitude += amplitude;
        amplitude *= persistence; 
        frequency *= lacunarity; 
    }

    return total / maxAmplitude;
}