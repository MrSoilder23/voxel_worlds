#include "chunk_manager.hpp"

void ChunkManager::InsertToChunk(Chunk& chunk, BlockTypes block, uint blockX, uint blockY, uint blockZ) {
    if(blockX > VoxelWorlds::CHUNK_SIZE || blockY > VoxelWorlds::CHUNK_SIZE|| blockZ > VoxelWorlds::CHUNK_SIZE) {
        return;
    }
    if(blockX < 0 || blockY < 0 || blockZ < 0) {
        return;
    }
    chunk.blocks[blockX][blockY][blockZ] = block;
}
BlockTypes ChunkManager::GetBlock(Chunk& chunk, uint blockX, uint blockY, uint blockZ) const{
    if(blockX < 0 || blockX > chunk.chunkSize) {
        return BlockTypes::air;
    } else if(blockY < 0 || blockY > chunk.chunkSize) {
        return BlockTypes::air;
    } else if(blockZ < 0 || blockZ > chunk.chunkSize) {
        return BlockTypes::air;
    }

    return chunk.blocks[blockX][blockY][blockZ];
}

void ChunkManager::AddModel(Chunk& chunk, Model model) {
    chunk.mModel = std::move(model);
}

void ChunkManager::CreateVAO(Chunk& chunk) {
    std::map<GLuint64, GLuint> mTextures;
    BlockTextureCreator& blockTexture = BlockTextureCreator::GetInstance();
    auto& textureList = blockTexture.GetTextures();
    
    for (const auto& [key, textureHandle] : textureList) { 
        if (mTextures.count(textureHandle) == 0) {
            mTextures[textureHandle];
        }
    }

    GLuint index = 0;
    for(auto& [handle, _] : mTextures) {
        mTextures[handle] = index++;
    }

    std::vector<GLuint64> handlers;
    for(auto& [textureId, _] : mTextures) {
        handlers.push_back(textureId);
    }
    GLuint vao, vbo, ebo, texId, textures, textureCoords;
    glGenVertexArrays(1, &chunk.mVertexArrayObject);
    glBindVertexArray(chunk.mVertexArrayObject);

    // Start generating VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, chunk.mModel.vertexPositions.size()*sizeof(glm::vec3), chunk.mModel.vertexPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk.mModel.indexBufferData.size()*sizeof(GLuint), chunk.mModel.indexBufferData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &textures);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, textures);
    glBufferData(GL_SHADER_STORAGE_BUFFER, handlers.size() * sizeof(GLuint64), handlers.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, textures);

    glGenBuffers(1, &textureCoords);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoords);
    glBufferData(GL_ARRAY_BUFFER, chunk.mTexturePositions.size() * sizeof(glm::vec3), chunk.mTexturePositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &texId);
    glBindBuffer(GL_ARRAY_BUFFER, texId);
    glBufferData(GL_ARRAY_BUFFER, chunk.mTextureID.size() * sizeof(GLuint), chunk.mTextureID.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}