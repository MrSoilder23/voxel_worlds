#pragma once
// C++ standard libraries
#include <iostream>
#include <memory>
#include <random>
#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>

// Third_Party libraries
#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

// Own Libraries
#include "./world/transform.hpp"
#include "./components/bounding_box_component.hpp"
#include "./components/position_component.hpp"
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

    // Position Component tools
    inline void MovePosition(PositionComponent& positionComponent, const glm::vec3& newPosition) {
        positionComponent.mPosition = newPosition;
        positionComponent.mDirty = true;
    }

    inline void RotatePosition(PositionComponent& positionComponent, const glm::quat& newRotation) {
        positionComponent.mRotation = newRotation;
        positionComponent.mDirty = true;
    }

    inline void ScalePosition(PositionComponent& positionComponent, const glm::vec3& newScale) {
        positionComponent.mScale = newScale;
        positionComponent.mDirty = true;
    }

    // Collision detection
    inline bool Instersects(const BoundingBoxComponent& box1, const BoundingBoxComponent& box2) {
        return (box1.mWorldMin.x <= box2.mWorldMax.x && box1.mWorldMax.x >= box2.mWorldMin.x) &&
               (box1.mWorldMin.y <= box2.mWorldMax.y && box1.mWorldMax.y >= box2.mWorldMin.y) &&
               (box1.mWorldMin.z <= box2.mWorldMax.z && box1.mWorldMax.z >= box2.mWorldMin.z);
    }

    inline glm::vec3 AxisOfLeastOverlap(const BoundingBoxComponent& box1, const BoundingBoxComponent& box2) {
        constexpr float epsilon = 0.001f;

        const float overlapX = std::min(box1.mWorldMax.x, box2.mWorldMax.x) - std::max(box1.mWorldMin.x, box2.mWorldMin.x);
        const float overlapY = std::min(box1.mWorldMax.y, box2.mWorldMax.y) - std::max(box1.mWorldMin.y, box2.mWorldMin.y);
        const float overlapZ = std::min(box1.mWorldMax.z, box2.mWorldMax.z) - std::max(box1.mWorldMin.z, box2.mWorldMin.z);
    
        glm::vec3 mtv(0.0f); // Minimum Translation Vector
    
        if (overlapX < overlapY && overlapX < overlapZ) {
            float direction = (box1.mWorldMin.x < box2.mWorldMin.x) ? 1.0f : -1.0f;
            mtv = glm::vec3(direction * (overlapX+epsilon), 0.0f, 0.0f);
        } else if (overlapY < overlapZ) {
            float direction = (box1.mWorldMin.y < box2.mWorldMin.y) ? 1.0f : -1.0f;
            mtv = glm::vec3(0.0f, direction * (overlapY+epsilon), 0.0f);
        } else {
            float direction = (box1.mWorldMin.z < box2.mWorldMin.z) ? 1.0f : -1.0f;
            mtv = glm::vec3(0.0f, 0.0f, direction * (overlapZ+epsilon));
        }
    
        return mtv;
    }

    // Works for only one object moving (first box)
    inline float SweptAABB(const BoundingBoxComponent& box1, const glm::vec3& velocity, const BoundingBoxComponent& box2, glm::vec3& normals) {
        if ((velocity.x == 0.0f && (box1.mWorldMax.x < box2.mWorldMin.x || box1.mWorldMin.x > box2.mWorldMax.x)) ||
            (velocity.y == 0.0f && (box1.mWorldMax.y < box2.mWorldMin.y || box1.mWorldMin.y > box2.mWorldMax.y)) ||
            (velocity.z == 0.0f && (box1.mWorldMax.z < box2.mWorldMin.z || box1.mWorldMin.z > box2.mWorldMax.z))) {
            normals = glm::vec3(0.0f);
            return 1.0f;
        }

        float xBoxEntry, yBoxEntry, zBoxEntry;
        float xBoxExit, yBoxExit, zBoxExit;

        // Calculate entry and exit distances
        if (velocity.x > 0.0f) {
            xBoxEntry = box2.mWorldMin.x - box1.mWorldMax.x;
            xBoxExit = box2.mWorldMax.x - box1.mWorldMin.x;
        } else {
            xBoxEntry = box2.mWorldMax.x - box1.mWorldMin.x;
            xBoxExit = box2.mWorldMin.x - box1.mWorldMax.x;
        }

        if (velocity.y > 0.0f) {
            yBoxEntry = box2.mWorldMin.y - box1.mWorldMax.y;
            yBoxExit = box2.mWorldMax.y - box1.mWorldMin.y;
        } else {
            yBoxEntry = box2.mWorldMax.y - box1.mWorldMin.y;
            yBoxExit = box2.mWorldMin.y - box1.mWorldMax.y;
        }

        if (velocity.z > 0.0f) {
            zBoxEntry = box2.mWorldMin.z - box1.mWorldMax.z;
            zBoxExit = box2.mWorldMax.z - box1.mWorldMin.z;
        } else {
            zBoxEntry = box2.mWorldMax.z - box1.mWorldMin.z;
            zBoxExit = box2.mWorldMin.z - box1.mWorldMax.z;
        }

        // Calculate entry/exit times
        float xEntry = (velocity.x != 0.0f) ? xBoxEntry / velocity.x : -std::numeric_limits<float>::infinity();
        float xExit = (velocity.x != 0.0f) ? xBoxExit / velocity.x : std::numeric_limits<float>::infinity();

        float yEntry = (velocity.y != 0.0f) ? yBoxEntry / velocity.y : -std::numeric_limits<float>::infinity();
        float yExit = (velocity.y != 0.0f) ? yBoxExit / velocity.y : std::numeric_limits<float>::infinity();

        float zEntry = (velocity.z != 0.0f) ? zBoxEntry / velocity.z : -std::numeric_limits<float>::infinity();
        float zExit = (velocity.z != 0.0f) ? zBoxExit / velocity.z : std::numeric_limits<float>::infinity();

        float entryTime = std::max(std::max(xEntry, yEntry), zEntry);
        float exitTime = std::min(std::min(xExit, yExit), zExit);

        // No collision conditions
        if (entryTime > exitTime || entryTime > 1.0f || exitTime < 0.0f) {
            normals = glm::vec3(0.0f);
            return 1.0f;
        }

        if (entryTime < 0.0f) {
            normals = glm::vec3(0.0f);
            return 0.0f;
        }

        // Determine collision normal
        if (xEntry > yEntry && xEntry > zEntry) { 
            normals.x = (velocity.x > 0.0f) ? -1.0f : 1.0f;
            normals.y = 0.0f; 
            normals.z = 0.0f; 
        } else if (yEntry > zEntry) { 
            normals.y = (velocity.y > 0.0f) ? -1.0f : 1.0f;
            normals.x = 0.0f; 
            normals.z = 0.0f; 
        } else { 
            normals.z = (velocity.z > 0.0f) ? -1.0f : 1.0f;
            normals.x = 0.0f; 
            normals.y = 0.0f; 
        }

        return entryTime;
    }

    // Bounding box model creation
    inline Model CreateBoundingModel(BoundingBoxComponent& boundingBox) {
        Model model;
        model.vertexPositions = {
            glm::vec3(boundingBox.mLocalMin.x, boundingBox.mLocalMin.y, boundingBox.mLocalMin.z),  // Bottom-left-back
            glm::vec3(boundingBox.mLocalMax.x, boundingBox.mLocalMin.y, boundingBox.mLocalMin.z),  // Bottom-right-back
            glm::vec3(boundingBox.mLocalMax.x, boundingBox.mLocalMax.y, boundingBox.mLocalMin.z),  // Top-right-back
            glm::vec3(boundingBox.mLocalMin.x, boundingBox.mLocalMax.y, boundingBox.mLocalMin.z),  // Top-left-back
            glm::vec3(boundingBox.mLocalMin.x, boundingBox.mLocalMin.y, boundingBox.mLocalMax.z),  // Bottom-left-front
            glm::vec3(boundingBox.mLocalMax.x, boundingBox.mLocalMin.y, boundingBox.mLocalMax.z),  // Bottom-right-front
            glm::vec3(boundingBox.mLocalMax.x, boundingBox.mLocalMax.y, boundingBox.mLocalMax.z),  // Top-right-front
            glm::vec3(boundingBox.mLocalMin.x, boundingBox.mLocalMax.y, boundingBox.mLocalMax.z)   // Top-left-front
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
        constexpr float inverseMax = 1.0f / 4294967296.0f;  // 1 / 2^32
        float normalized = h * inverseMax;
        
        // Convert to an angle in [0, 2π)
        float angle = normalized * 6.2831853;
        
        return glm::vec2(std::cos(angle), std::sin(angle));
    }

    inline float Smooth(float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    inline float PerlinNoiseNormalized(int chunkX, int chunkY,float x, float y, unsigned int seed) {
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

        return result;
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

            float noise = PerlinNoiseNormalized(newChunkX, newChunkY, localX, localY, seed);
            total += noise * amplitude;

            maxAmplitude += amplitude;
            amplitude *= persistence; 
            frequency *= lacunarity; 
        }

        return total / maxAmplitude;
    }


}
