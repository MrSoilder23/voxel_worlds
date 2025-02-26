#include "./world/chunk_manager.hpp"

void ChunkManager::InsertToChunk(Chunk& chunk, BlockTypes block, uint blockX, uint blockY, uint blockZ) {
    if(blockX > VoxelWorlds::CHUNK_SIZE || blockY > VoxelWorlds::CHUNK_SIZE|| blockZ > VoxelWorlds::CHUNK_SIZE) {
        return;
    }
    if(blockX < 0 || blockY < 0 || blockZ < 0) {
        return;
    }
    chunk.blocks[blockX][blockY][blockZ] = block;
}
BlockTypes ChunkManager::GetBlock(Chunk& chunk, uint blockX, uint blockY, uint blockZ) {
    if(blockX < 0 || blockX > VoxelWorlds::CHUNK_SIZE) {
        return BlockTypes::air;
    } else if(blockY < 0 || blockY > VoxelWorlds::CHUNK_SIZE) {
        return BlockTypes::air;
    } else if(blockZ < 0 || blockZ > VoxelWorlds::CHUNK_SIZE) {
        return BlockTypes::air;
    }

    return chunk.blocks[blockX][blockY][blockZ];
}

void ChunkManager::AddModel(Chunk& chunk, Model model) {
    chunk.mModel = std::move(model);
}
#include <algorithm>
void ChunkManager::CreateVAO(Chunk& chunk) {
    std::map<GLuint64, GLuint> mTextures;
    BlockTextureCreator& blockTexture = BlockTextureCreator::GetInstance();
    std::vector<GLuint64> textures;

    for (const auto& pair : blockTexture.GetTextures()) {
        textures.push_back(pair.second);
    }
    std::sort(textures.begin(), textures.end());

    if(chunk.mVertexArrayObject != 0) {
        glDeleteVertexArrays(1, &chunk.mVertexArrayObject);
        glDeleteBuffers(1, &chunk.mVbo);
        glDeleteBuffers(1, &chunk.mEbo);
        glDeleteBuffers(1, &chunk.mTexturesBuffer);
        glDeleteBuffers(1, &chunk.mTextureCoords);
        glDeleteBuffers(1, &chunk.mTexId);
    }

    glGenVertexArrays(1, &chunk.mVertexArrayObject);
    glBindVertexArray(chunk.mVertexArrayObject);

    // Start generating VBO
    glGenBuffers(1, &chunk.mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, chunk.mVbo);
    glBufferData(GL_ARRAY_BUFFER, chunk.mModel.vertexPositions.size()*sizeof(glm::vec3), chunk.mModel.vertexPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &chunk.mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk.mModel.indexBufferData.size()*sizeof(GLuint), chunk.mModel.indexBufferData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &chunk.mTexturesBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, chunk.mTexturesBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, textures.size() * sizeof(GLuint64), textures.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, chunk.mTexturesBuffer);

    glGenBuffers(1, &chunk.mTextureCoords);
    glBindBuffer(GL_ARRAY_BUFFER, chunk.mTextureCoords);
    glBufferData(GL_ARRAY_BUFFER, chunk.mTexturePositions.size() * sizeof(glm::vec3), chunk.mTexturePositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &chunk.mTexId);
    glBindBuffer(GL_ARRAY_BUFFER, chunk.mTexId);
    glBufferData(GL_ARRAY_BUFFER, chunk.mTextureID.size() * sizeof(GLuint), chunk.mTextureID.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}