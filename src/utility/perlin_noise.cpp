#include "./utility/perlin_noise.hpp"

glm::vec2 perlin_noise::Gradient(int x, int y, unsigned int seed) {
    static constexpr size_t LOOKUP_SIZE = 4096;
    static constexpr float ANGLE_STEP = 2.0f * glm::pi<float>() / LOOKUP_SIZE;
    static const auto gradientLookUp = []() {
        std::array<glm::vec2, LOOKUP_SIZE> LookUpTable;
        for (size_t i = 0; i < LOOKUP_SIZE; ++i) {
            float angle = i * ANGLE_STEP;
            LookUpTable[i] = {std::cos(angle), std::sin(angle)};
        }
        return LookUpTable;
    }();

    uint32_t h = seed;
    h ^= static_cast<uint32_t>(x) * 374761393u;
    h ^= static_cast<uint32_t>(y) * 668265263u;
    h = (h ^ (h >> 13)) * 1274126177u;
    h ^= (h >> 16);
    
    return gradientLookUp[(h >> 20) & (LOOKUP_SIZE - 1)];
}

float perlin_noise::Noise2DNormalized(int chunkX, int chunkY,float x, float y, unsigned int seed) {
    float dot1 = glm::dot(Gradient(chunkX,chunkY , seed), glm::vec2(x,-y));
    float dot2 = glm::dot(Gradient(chunkX+1,chunkY, seed), glm::vec2(x-1.0f,-y));
    float dot3 = glm::dot(Gradient(chunkX,chunkY+1, seed), glm::vec2(x,1.0f-y));
    float dot4 = glm::dot(Gradient(chunkX+1,chunkY+1, seed), glm::vec2(x-1.0f,1.0f-y));

    x = utility::Smooth(x);
    y = utility::Smooth(y);

    float AB = dot1 + (x * (dot2-dot1));
    float CD = dot3 + (x * (dot4-dot3));

    float result = AB + (y * (CD-AB));
    result = (result + 1.0f) / 2.0f;

    return result * 1.414f;
}

float perlin_noise::Noise2D(int chunkX, int chunkY,float x, float y, unsigned int seed) {
    float dot1 = glm::dot(Gradient(chunkX,chunkY , seed), glm::vec2(x, y));
    float dot2 = glm::dot(Gradient(chunkX+1,chunkY, seed), glm::vec2(x-1.0f, y));
    float dot3 = glm::dot(Gradient(chunkX,chunkY+1, seed), glm::vec2(x, y-1.0f));
    float dot4 = glm::dot(Gradient(chunkX+1,chunkY+1, seed), glm::vec2(x-1.0f, y-1.0f));

    x = utility::Smooth(x);
    y = utility::Smooth(y);

    float AB = dot1 + (x * (dot2-dot1));
    float CD = dot3 + (x * (dot4-dot3));

    float result = AB + (y * (CD-AB));

    return result * 1.414f;
}

// Persistance - More Pronouced peaks and valleys
// Lacunarity -  More detailed terrain
float perlin_noise::LayeredNoise2DNormalized(int chunkX, int chunkY, float x, float y, unsigned int seed, int octaves, float persistence, float lacunarity) {
    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f; 
    float maxAmplitude = 0.0f;

    for (int i = 0; i < octaves; ++i) {
        float scaledX = (chunkX * VoxelWorlds::CHUNK_SIZE + x * VoxelWorlds::CHUNK_SIZE) * frequency;
        float scaledY = (chunkY * VoxelWorlds::CHUNK_SIZE + y * VoxelWorlds::CHUNK_SIZE) * frequency;

        int newChunkX = static_cast<int>(floor(scaledX / VoxelWorlds::CHUNK_SIZE));
        int newChunkY = static_cast<int>(floor(scaledY / VoxelWorlds::CHUNK_SIZE));
        float localX = (scaledX - newChunkX * VoxelWorlds::CHUNK_SIZE) / VoxelWorlds::CHUNK_SIZE;
        float localY = (scaledY - newChunkY * VoxelWorlds::CHUNK_SIZE) / VoxelWorlds::CHUNK_SIZE;

        float noise = Noise2DNormalized(newChunkX, newChunkY, localX, localY, seed);
        total += noise * amplitude;

        maxAmplitude += amplitude;
        amplitude *= persistence; 
        frequency *= lacunarity; 
    }

    return total / maxAmplitude;
}
float perlin_noise::LayeredNoise2D(int chunkX, int chunkY, float x, float y, unsigned int seed, int octaves, float persistence, float lacunarity) {
    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f; 
    float maxAmplitude = 0.0f;

    for (int i = 0; i < octaves; ++i) {
        float scaledX = (chunkX * VoxelWorlds::CHUNK_SIZE + x * VoxelWorlds::CHUNK_SIZE) * frequency;
        float scaledY = (chunkY * VoxelWorlds::CHUNK_SIZE + y * VoxelWorlds::CHUNK_SIZE) * frequency;

        int newChunkX = static_cast<int>(floor(scaledX / VoxelWorlds::CHUNK_SIZE));
        int newChunkY = static_cast<int>(floor(scaledY / VoxelWorlds::CHUNK_SIZE));
        float localX = (scaledX - newChunkX * VoxelWorlds::CHUNK_SIZE) / VoxelWorlds::CHUNK_SIZE;
        float localY = (scaledY - newChunkY * VoxelWorlds::CHUNK_SIZE) / VoxelWorlds::CHUNK_SIZE;

        float noise = Noise2D(newChunkX, newChunkY, localX, localY, seed);
        total += noise * amplitude;

        maxAmplitude += amplitude;
        amplitude *= persistence; 
        frequency *= lacunarity; 
    }

    return total / maxAmplitude;
}