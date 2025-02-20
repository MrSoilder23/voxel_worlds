#pragma once
// C++ standard libraries
#include <iostream>
#include <memory>
#include <random>
#define _USE_MATH_DEFINES
#include <cmath>

// Third_Party libraries
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Own Libraries
#include "./world/transform.hpp"
#include "./components/bounding_box_component.hpp"
#include "constant.hpp"
#include "./model.hpp"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

namespace utility {
    
    // Debug
    inline void GetOpenGlVersionInfo() {
        std::cout << "Vendor: "   <<         glGetString(GL_VENDOR)           << std::endl;
        std::cout << "Renderer: " <<         glGetString(GL_RENDERER)         << std::endl;
        std::cout << "Version: "  <<         glGetString(GL_VERSION)          << std::endl;
        std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    }

    // Transform
    inline void MeshTranslate(Transform& transform, float x, float y, float z) {
        transform.mModelMatrix = glm::translate(transform.mModelMatrix, glm::vec3(x,y,z));
    }
    inline void MeshTranslate(Transform& transform, glm::vec3 transformation) {
        transform.mModelMatrix = glm::translate(transform.mModelMatrix, transformation);
    }

    inline void MeshRotate(Transform& transform, float angle, glm::vec3 axis) {
        transform.mModelMatrix = glm::rotate(transform.mModelMatrix, angle, axis);
    }

    inline void MeshScale(Transform& transform, float x, float y, float z) {
        transform.mModelMatrix = glm::scale(transform.mModelMatrix, glm::vec3(x,y,z));
    }

    // Collision detection
    inline bool Instersects(const BoundingBoxComponent& box1, const BoundingBoxComponent& box2) {
        return (box1.mMin.x <= box2.mMax.x && box1.mMax.x >= box2.mMin.x) &&
               (box1.mMin.y <= box2.mMax.y && box1.mMax.y >= box2.mMin.y) &&
               (box1.mMin.z <= box2.mMax.z && box1.mMax.z >= box2.mMin.z);
    }

    // Bounding box model creation
    inline Model CreateBoundingModel(BoundingBoxComponent& boundingBox) {
        Model model;
        model.vertexPositions = {
            glm::vec3(boundingBox.mMin.x, boundingBox.mMin.y, boundingBox.mMin.z),  // Bottom-left-back
            glm::vec3(boundingBox.mMax.x, boundingBox.mMin.y, boundingBox.mMin.z),  // Bottom-right-back
            glm::vec3(boundingBox.mMax.x, boundingBox.mMax.y, boundingBox.mMin.z),  // Top-right-back
            glm::vec3(boundingBox.mMin.x, boundingBox.mMax.y, boundingBox.mMin.z),  // Top-left-back
            glm::vec3(boundingBox.mMin.x, boundingBox.mMin.y, boundingBox.mMax.z),  // Bottom-left-front
            glm::vec3(boundingBox.mMax.x, boundingBox.mMin.y, boundingBox.mMax.z),  // Bottom-right-front
            glm::vec3(boundingBox.mMax.x, boundingBox.mMax.y, boundingBox.mMax.z),  // Top-right-front
            glm::vec3(boundingBox.mMin.x, boundingBox.mMax.y, boundingBox.mMax.z)   // Top-left-front
        };

        model.indexBufferData = {
            0, 1,  // Bottom edge
            1, 2,  // Right edge
            2, 3,  // Top edge
            3, 0,  // Left edge
        
            // Front face edges
            4, 5,  // Bottom edge
            5, 6,  // Right edge
            6, 7,  // Top edge
            7, 4,  // Left edge
        
            // Connecting edges between back and front faces
            0, 4,  // Bottom-left edge
            1, 5,  // Bottom-right edge
            2, 6,  // Top-right edge
            3, 7   // Top-left edge
        };

        return model;
    }

    // World Generation
    inline glm::vec2 Gradient(int x, int y, unsigned int seed) {
        uint32_t h = seed;
        h ^= static_cast<uint32_t>(x) * 374761393u;
        h ^= static_cast<uint32_t>(y) * 668265263u;
        h = (h ^ (h >> 13)) * 1274126177u;
        h ^= (h >> 16);
        
        // Map h to a float in [0, 1)
        const float invMax = 1.0f / 4294967296.0f;  // 1 / 2^32
        float normalized = h * invMax;
        
        // Convert to an angle in [0, 2π)
        float angle = normalized * 6.2831853;
        
        return glm::vec2(std::cos(angle), std::sin(angle));
    }

    inline float Smooth(float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    inline float PerlinNoise(int chunkX, int chunkY,float x, float y, unsigned int seed) {
        float dot1 = glm::dot(Gradient(chunkX,chunkY , seed), glm::vec2(x,-y));
        float dot2 = glm::dot(Gradient(chunkX+1,chunkY, seed), glm::vec2(x-1.0f,-y));
        float dot3 = glm::dot(Gradient(chunkX,chunkY+1, seed), glm::vec2(x,1.0f-y));
        float dot4 = glm::dot(Gradient(chunkX+1,chunkY+1, seed), glm::vec2(x-1.0f,1.0f-y));

        x = Smooth(x);
        y = Smooth(y);

        float AB = dot1 + (x * (dot2-dot1));
        float CD = dot3 + (x * (dot4-dot3));

        float result = AB + (y * (CD-AB));

        return (result + 1.0f) / 2.0f;
    }

    // Persistance - More Pronouced peaks and valleys
    // Lacunarity -  More detailed terrain
    inline float LayeredPerlinNoise(int chunkX, int chunkY, float x, float y, unsigned int seed, int octaves = 4, float persistence = 0.5f, float lacunarity = 2.0f) {
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

            float noise = PerlinNoise(newChunkX, newChunkY, localX, localY, seed);
            total += noise * amplitude;

            maxAmplitude += amplitude;
            amplitude *= persistence; 
            frequency *= lacunarity; 
        }

        return total / maxAmplitude;
    }


}
