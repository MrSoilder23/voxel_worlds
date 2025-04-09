#include "./utility/world_generation.hpp"

float world_generation::GenerateHeight(const unsigned int& seed, int x, int z) {
    x = x + 1343;
    z = z + 343;
    float continentalness = open_simplex_noise::LayeredNoise2D(
        x * (0.005f / (4 * VoxelWorlds::SCALE)),
        z * (0.005f / (4 * VoxelWorlds::SCALE)),
        seed,
        4,
        0.5f,
        2.5f
    );
    float erosion = open_simplex_noise::LayeredNoise2D(
        x * (0.004f / (4 * VoxelWorlds::SCALE)),
        z * (0.004f / (4 * VoxelWorlds::SCALE)),
        seed,
        4,
        0.4f,
        2.4f
    );
    float peaksAndValleys = open_simplex_noise::LayeredNoise2D(
        x * (0.025f / (10 * VoxelWorlds::SCALE)),
        z * (0.025f / (10 * VoxelWorlds::SCALE)),
        seed,
        6,
        0.4f,
        1.8f
    );

    float continentalAdj = VoxelWorlds::CONTINENTAL_SPLINE.evaluate(continentalness);
    float erosionAdj = VoxelWorlds::EROSION_SPLINE.evaluate(erosion);
    float peaksValleysAdj = VoxelWorlds::PEAKS_VALLEYS_SPLINE.evaluate(peaksAndValleys);

    return (continentalAdj * 0.5f) + (erosionAdj * 0.3f) + (peaksValleysAdj * 0.2f);
}