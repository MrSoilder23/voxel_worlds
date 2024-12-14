#pragma once
// C++ standard libraries
#include <vector>

// Third_party libraries
#include <glad/glad.h>

struct Model {
    std::vector<GLfloat> vertexPositions;
    std::vector<GLuint> indexBufferData;
};