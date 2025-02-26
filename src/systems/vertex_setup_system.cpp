#include "./systems/vertex_setup_system.hpp"

void VertexSetupSystem::CreateVertexSpecification(EntityManager& entityManager) {
    for(const auto& componentPointer : entityManager.GetEntities()) {
        auto model = entityManager.GetComponent<ModelComponent>(componentPointer.first);
        auto boundingBox = entityManager.GetComponent<BoundingBoxComponent>(componentPointer.first);

        if(model && model->VAO == 0) {
            
            glGenVertexArrays(1, &model->VAO);
            glBindVertexArray(model->VAO);
            
            glGenBuffers(1, &model->VBO);
            glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
            glBufferData(GL_ARRAY_BUFFER, model->mModel.vertexPositions.size()*sizeof(glm::vec3), model->mModel.vertexPositions.data(), GL_STATIC_DRAW);
            
            glGenBuffers(1, &model->EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->mModel.indexBufferData.size()*sizeof(GLuint), model->mModel.indexBufferData.data(), GL_STATIC_DRAW);
            
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
            glBindVertexArray(0);
            
        } else if(boundingBox && boundingBox->VAO == 0) {
            glGenVertexArrays(1, &boundingBox->VAO);
            glBindVertexArray(boundingBox->VAO);
            
            glGenBuffers(1, &boundingBox->VBO);
            glBindBuffer(GL_ARRAY_BUFFER, boundingBox->VBO);
            glBufferData(GL_ARRAY_BUFFER, boundingBox->mModel.vertexPositions.size()*sizeof(glm::vec3), boundingBox->mModel.vertexPositions.data(), GL_STATIC_DRAW);
            
            glGenBuffers(1, &boundingBox->EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundingBox->EBO);
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

    if(model && model->VAO == 0) {
            
        glGenVertexArrays(1, &model->VAO);
        glBindVertexArray(model->VAO);
        
        glGenBuffers(1, &model->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
        glBufferData(GL_ARRAY_BUFFER, model->mModel.vertexPositions.size()*sizeof(glm::vec3), model->mModel.vertexPositions.data(), GL_STATIC_DRAW);
        
        glGenBuffers(1, &model->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->mModel.indexBufferData.size()*sizeof(GLuint), model->mModel.indexBufferData.data(), GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glBindVertexArray(0);
        
    } else if(boundingBox && boundingBox->VAO == 0) {
        glGenVertexArrays(1, &boundingBox->VAO);
        glBindVertexArray(boundingBox->VAO);
        
        glGenBuffers(1, &boundingBox->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, boundingBox->VBO);
        glBufferData(GL_ARRAY_BUFFER, boundingBox->mModel.vertexPositions.size()*sizeof(glm::vec3), boundingBox->mModel.vertexPositions.data(), GL_STATIC_DRAW);
        
        glGenBuffers(1, &boundingBox->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boundingBox->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, boundingBox->mModel.indexBufferData.size()*sizeof(GLuint), boundingBox->mModel.indexBufferData.data(), GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glBindVertexArray(0);
    }
}