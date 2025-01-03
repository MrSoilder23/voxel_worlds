#pragma once
// C++ standard libraries
#include <vector>

// Third_party libraries
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Model {
    std::vector<glm::vec3> vertexPositions;
    std::vector<GLuint> indexBufferData;
};