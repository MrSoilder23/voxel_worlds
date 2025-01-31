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
#include "transform.hpp"

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

    // World Generation
    inline glm::vec2 Gradient(int x, int y, unsigned int seed) {
        std::mt19937 mersenneEngine{seed + x * 985712 ^ y * 5019858};
        std::uniform_real_distribution<> dist{0, 2*M_PI};
        
        float a = dist(mersenneEngine);
        return glm::vec2(std::cos(a), std::sin(a));
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

}
