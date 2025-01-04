#include "chunk.hpp"

Chunk::Chunk() {
    mModel = std::make_shared<Model>();
}

void Chunk::CreateVao() {
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

    for (int x = 0; x < 32; x++) {
        for (int y = 0; y < 32; y++) {
            for (int z = 0; z < 32; z++) {
                auto modelComponentLocation = blocks[x][y][z].find(std::type_index(typeid(ModelComponent)));
                if (modelComponentLocation == blocks[x][y][z].end()) {
                    continue;
                }

                auto modelComponent = std::dynamic_pointer_cast<ModelComponent>(modelComponentLocation->second);

                std::vector<glm::vec3>& vertexPositions = modelComponent->GetModel().vertexPositions;
                std::vector<GLuint>& indexBufferData = modelComponent->GetModel().indexBufferData;

                glm::mat4& transform = modelComponent->GetTransform().mModelMatrix;

                if(indexBufferData.size() == 0) {
                    continue;
                }

                size_t modelSize = mModel->vertexPositions.size();

                for (size_t i = 0; i < vertexPositions.size(); i++) {
                    glm::vec3 vertex = glm::vec3(transform * glm::vec4(vertexPositions[i], 1.0f));
                    
                    mModel->vertexPositions.push_back(vertex);
                }

                for (size_t i = 0; i < indexBufferData.size(); i++) {
                    mModel->indexBufferData.push_back(indexBufferData[i] + (modelSize));
                }

                mTexturePositions.insert(mTexturePositions.end(), vertexPositions.begin(), vertexPositions.end());
                mTextureID.insert(mTextureID.end(), vertexPositions.size(), mTextures[modelComponent->GetTexture()->textureHandle]);
            }
        }
    }
    std::vector<GLuint64> handlers;
    for(auto& [textureId, _] : mTextures) {
        handlers.push_back(textureId);
    }
    
    GLuint vao, vbo, ebo, texId, textures, textureCoords;
    glGenVertexArrays(1, &mMeshData.GetVAO());
    glBindVertexArray(mMeshData.GetVAO());

    // Start generating VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, mModel->vertexPositions.size()*sizeof(glm::vec3), mModel->vertexPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mModel->indexBufferData.size()*sizeof(GLuint), mModel->indexBufferData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &textures);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, textures);
    glBufferData(GL_SHADER_STORAGE_BUFFER, handlers.size() * sizeof(GLuint64), handlers.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, textures);

    glGenBuffers(1, &textureCoords);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoords);
    glBufferData(GL_ARRAY_BUFFER, mTexturePositions.size() * sizeof(glm::vec3), mTexturePositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &texId);
    glBindBuffer(GL_ARRAY_BUFFER, texId);
    glBufferData(GL_ARRAY_BUFFER, mTextureID.size() * sizeof(GLuint), mTextureID.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
std::shared_ptr<Model>& Chunk::GetModel() {
    return mModel;
}
MeshData& Chunk::GetMeshData() {
    return mMeshData;
}
Transform& Chunk::GetTransform() {
    return mTransform;
}