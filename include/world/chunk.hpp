#pragma once
// C++ standard libraries
#include <map>
#include <iostream>
#include <mutex>

// Third_party libraries
#include <glm/glm.hpp>
#include <glad/glad.h>

// Own libraries
#include "./blocks/block_types.hpp"
#include "model.hpp"
#include "transform.hpp"
#include "./utility/constant.hpp"

struct Chunk {
    BlockTypes blocks[static_cast<size_t>(VoxelWorlds::CHUNK_SIZE)][static_cast<size_t>(VoxelWorlds::CHUNK_SIZE)][static_cast<size_t>(VoxelWorlds::CHUNK_SIZE)];
    bool wasGenerated = false;
    
    Model mModel;
    std::map<GLuint64, GLuint> mTextures;
    
    std::vector<glm::vec3> mTexturePositions;
    std::vector<GLuint> mTextureID;

    GLuint mVertexArrayObject;
    GLuint mVbo, mEbo, mTexId, mTexturesBuffer, mTextureCoords;
    Transform mTransform;
};