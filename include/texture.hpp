#pragma once
// C++ standard libraries
#include <vector>

// Third_party libraries
#include <glad/glad.h>

struct Texture {
    Texture() = default;
    inline Texture(std::vector<GLfloat> _colorPositions) {
        colorPositions = _colorPositions;
    }

    std::vector<GLfloat> colorPositions;  
};