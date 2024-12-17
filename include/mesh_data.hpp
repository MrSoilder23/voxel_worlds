#pragma once
// Third_party libraries
#include <glad/glad.h>

// Own libraries
#include "transform.hpp"

struct MeshData {
    GLuint mVertexArrayObject = 0;
    GLuint mVertexBufferObject = 0;
    GLuint mVertexBufferObjectColor = 0;

    GLuint mIndexBufferObject = 0;

    Transform mTransform;
};