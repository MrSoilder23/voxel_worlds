#pragma once
// C++ standard libraries
#include <functional>

// Third_party libraries
#include <glm/glm.hpp>

struct IVec3Hash {
    size_t operator()(const glm::ivec3& k) const {
        // Combine the hash values of x, y, and z
        size_t seed = 0;
        seed ^= std::hash<int>()(k.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>()(k.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>()(k.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

struct IVec2Hash {
    size_t operator()(const glm::ivec2& k) const {
        // Combine the hash values of x, and y
        size_t seed = 0;
        seed ^= std::hash<int>()(k.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>()(k.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};
