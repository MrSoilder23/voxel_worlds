#include "./systems/vertex_setup_system.hpp"

void VertexSetupSystem::CreateVertexSpecification(EntityManager& entityManager) {
    for(const auto& componentPointer : entityManager.GetEntities()) {
        auto model = entityManager.GetComponent<ModelComponent>(componentPointer.first);
        auto boundingBox = entityManager.GetComponent<BoundingBoxComponent>(componentPointer.first);

        if(model && model->mVAO == 0) {
            
            glGenVertexArrays(1, &model->mVAO);
            glBindVertexArray(model->mVAO);
            
            glGenBuffers(1, &model->mVBO);
            glBindBuffer(GL_ARRAY_BUFFER, model->mVBO);
            glBufferData(GL_ARRAY_BUFFER, model->mModel.vertexPositions.size()*sizeof(glm::vec3), model->mModel.vertexPositions.data(), GL_STATIC_DRAW);
            
            glGenBuffers(1, &model->mEBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->mEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->mModel.indexBufferData.size()*sizeof(GLuint), model->mModel.indexBufferData.data(), GL_STATIC_DRAW);
            
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
            glBindVertexArray(0);
            
        } else if(boundingBox && boundingBox->mVAO == 0) {
            glGenVertexArrays(1, &boundingBox->mVAO);
            glBindVertexArray(boundingBox->mVAO);
            
            glGenBuffers(1, &boundingBox->mVBO);
            glBindBuffer(GL_ARRAY_BUFFER, boundingBox->mVBO);
            glBufferData(GL_ARRAY_BUFFER, boundingBox->mModel.vertexPositions.size()*sizeof(glm::vec3), boundingBox->mModel.vertexPositions.data(), GL_STATIC_DRAW);
            
            glGenBuffers(1, &boundingBox->mEBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundingBox->mEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, boundingBox->mModel.indexBufferData.size()*sizeof(GLuint), boundingBox->mModel.indexBufferData.data(), GL_STATIC_DRAW);
            
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
            glBindVertexArray(0);
        }
    }
}

void VertexSetupSystem::CreateVertexSpecificationSingle(EntityManager& entityManager, std::string entityName) {
    auto model = entityManager.GetComponent<ModelComponent>(entityName);
    auto boundingBox = entityManager.GetComponent<BoundingBoxComponent>(entityName);

    if(model && model->mVAO == 0) {
            
        glGenVertexArrays(1, &model->mVAO);
        glBindVertexArray(model->mVAO);
        
        glGenBuffers(1, &model->mVBO);
        glBindBuffer(GL_ARRAY_BUFFER, model->mVBO);
        glBufferData(GL_ARRAY_BUFFER, model->mModel.vertexPositions.size()*sizeof(glm::vec3), model->mModel.vertexPositions.data(), GL_STATIC_DRAW);
        
        glGenBuffers(1, &model->mEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->mModel.indexBufferData.size()*sizeof(GLuint), model->mModel.indexBufferData.data(), GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glBindVertexArray(0);
        
    } else if(boundingBox && boundingBox->mVAO == 0) {
        glGenVertexArrays(1, &boundingBox->mVAO);
        glBindVertexArray(boundingBox->mVAO);
        
        glGenBuffers(1, &boundingBox->mVBO);
        glBindBuffer(GL_ARRAY_BUFFER, boundingBox->mVBO);
        glBufferData(GL_ARRAY_BUFFER, boundingBox->mModel.vertexPositions.size()*sizeof(glm::vec3), boundingBox->mModel.vertexPositions.data(), GL_STATIC_DRAW);
        
        glGenBuffers(1, &boundingBox->mEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundingBox->mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, boundingBox->mModel.indexBufferData.size()*sizeof(GLuint), boundingBox->mModel.indexBufferData.data(), GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glBindVertexArray(0);
    }
}