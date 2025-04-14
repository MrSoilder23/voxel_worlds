#include "./systems/chunk_vertex_setup_system.hpp"

void ChunkVertexSetupSystem::CreateVertexSpecification(EntityManager& entityManager) {
    auto models = entityManager.GetComponentArray<ChunkModelComponent>();

    for(size_t entityID = 0; entityID < models.size(); entityID++) {

        if(entityID >= models.size()) {
            continue;
        }

        auto model = models[entityID];
        
        if(model && !model->mGenerated && model->mModel.vertexPositions.size() != 0) {

            if(model->mVAO != 0) {
                glDeleteVertexArrays(1, &model->mVAO);
                glDeleteBuffers(1, &model->mVBO);
                glDeleteBuffers(1, &model->mEBO);
                glDeleteBuffers(1, &model->mTexCoords);

                model->mVAO = 0;
                model->mVBO = 0;
                model->mEBO = 0;
                model->mTexCoords = 0;
            }

            glGenVertexArrays(1, &model->mVAO);
            glBindVertexArray(model->mVAO);

            // Start generating VBO
            glGenBuffers(1, &model->mVBO);
            glBindBuffer(GL_ARRAY_BUFFER, model->mVBO);
            glBufferData(GL_ARRAY_BUFFER, model->mModel.vertexPositions.size()*sizeof(glm::vec3), model->mModel.vertexPositions.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

            glGenBuffers(1, &model->mEBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->mEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->mModel.indexBufferData.size()*sizeof(GLuint), model->mModel.indexBufferData.data(), GL_STATIC_DRAW);

            glGenBuffers(1, &model->mTexCoords);
            glBindBuffer(GL_ARRAY_BUFFER, model->mTexCoords);
            glBufferData(GL_ARRAY_BUFFER, model->mTexturePositions.size() * sizeof(glm::vec2), model->mTexturePositions.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, (void*)0);

            glBindVertexArray(0);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);

            model->mGenerated = true;
        }
    }
}

void ChunkVertexSetupSystem::CreateVertexSpecificationSingle(EntityManager& entityManager, std::string entityName) {
    auto model = entityManager.GetComponent<ChunkModelComponent>(entityName);

    if(model && !model->mGenerated && model->mModel.vertexPositions.size() != 0) {
        BlockTextureCreator& blockTexture = BlockTextureCreator::GetInstance();

        std::vector<GLuint64> textures;

        for (const auto& pair : blockTexture.GetTextures()) {
            textures.push_back(pair.second);
        }
        std::sort(textures.begin(), textures.end());

        if(model->mVAO != 0) {
            glDeleteVertexArrays(1, &model->mVAO);
            glDeleteBuffers(1, &model->mVBO);
            glDeleteBuffers(1, &model->mEBO);
            glDeleteBuffers(1, &model->mTexCoords);

            model->mVAO = 0;
            model->mVBO = 0;
            model->mEBO = 0;
            model->mTexCoords = 0;
        }

        glGenVertexArrays(1, &model->mVAO);
        glBindVertexArray(model->mVAO);

        // Start generating VBO
        glGenBuffers(1, &model->mVBO);
        glBindBuffer(GL_ARRAY_BUFFER, model->mVBO);
        glBufferData(GL_ARRAY_BUFFER, model->mModel.vertexPositions.size()*sizeof(glm::vec3), model->mModel.vertexPositions.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

        glGenBuffers(1, &model->mEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->mModel.indexBufferData.size()*sizeof(GLuint), model->mModel.indexBufferData.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &model->mTexCoords);
        glBindBuffer(GL_ARRAY_BUFFER, model->mTexCoords);
        glBufferData(GL_ARRAY_BUFFER, model->mTexturePositions.size() * sizeof(glm::vec3), model->mTexturePositions.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (void*)0);

        glEnableVertexAttribArray(2);
        glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 0, (void*)0);

        glBindVertexArray(0);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        
        model->mGenerated = true;
    }
}