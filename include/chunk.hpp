#pragma once
// C++ standard libraries
#include <map>
#include <iostream>

// Third_party libraries
#include <glm/glm.hpp>
#include <glad/glad.h>

// Own libraries
#include "block_types.hpp"
#include "model.hpp"
#include "transform.hpp"

struct Chunk {
    BlockTypes blocks[32][32][32];
    int chunkSize;

    Model mModel;
    std::map<GLuint64, GLuint> mTextures;
    
    std::vector<glm::vec3> mTexturePositions;
    std::vector<GLuint> mTextureID;

    GLuint mVertexArrayObject;
    Transform mTransform;
};