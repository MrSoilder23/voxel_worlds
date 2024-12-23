#pragma once
// C++ standard libraries
#include <vector>
#include <memory>

// Third_party libraries
#include <SDL2/SDL_image.h>
#include <glad/glad.h>

struct Texture {
    inline Texture() {
        colorPositions = {
            1.0f,  0.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
            0.0f,  0.0f, 1.0f,
            0.0f,  0.0f, 1.0f,

            1.0f,  0.0f, 0.0f,
            0.0f,  0.0f, 1.0f,
            0.0f,  1.0f, 0.0f,
            1.0f,  0.0f, 0.0f,
        };
    }

    std::vector<GLfloat> colorPositions;  
    unsigned int textureID;
};