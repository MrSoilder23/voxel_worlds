#pragma once
// C++ standard libraries
#include <vector>

// Third_party libraries
#include <glm/glm.hpp>
#include <glad/glad.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec4 color;
};

struct MeshComponent {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    bool isDirty = true;
};