#include "model_component.hpp"
#include <iostream>

ModelComponent::ModelComponent() {
    // Default textures
    mTextures = std::make_shared<Texture>(std::vector<GLfloat>{
         1.0f,  0.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,

         1.0f,  0.0f, 0.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  1.0f, 0.0f,
         1.0f,  0.0f, 0.0f,
    });
         
}
ModelComponent::~ModelComponent() {
    std::cout << "ASDADA1" << std::endl;
    glDeleteBuffers(1, &mMeshData.mVertexArrayObject);
    glDeleteBuffers(1, &mMeshData.mVertexBufferObjectColor);
    glDeleteBuffers(1, &mMeshData.mIndexBufferObject);

    glDeleteVertexArrays(1, &mMeshData.mVertexArrayObject);
}

void ModelComponent::Initialize() {

    glGenVertexArrays(1, &mMeshData.mVertexArrayObject);
    glBindVertexArray(mMeshData.mVertexArrayObject);

    glGenBuffers(1, &mMeshData.mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mMeshData.mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, mModel->vertexPositions.size()*sizeof(GLfloat), mModel->vertexPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &mMeshData.mIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mMeshData.mIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mModel->indexBufferData.size()*sizeof(GLuint), mModel->indexBufferData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mMeshData.mVertexBufferObjectColor);
    glBindBuffer(GL_ARRAY_BUFFER, mMeshData.mVertexBufferObjectColor);
    glBufferData(GL_ARRAY_BUFFER, mTextures->colorPositions.size()*sizeof(GLfloat), mTextures->colorPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void ModelComponent::AddModel(const std::shared_ptr<Model>& model) {
    mModel = model;
}
void ModelComponent::AddTextures(const std::shared_ptr<Texture>& textures) {
    mTextures = textures;
}

std::shared_ptr<Model>& ModelComponent::GetModel() {
    return mModel;
}
std::shared_ptr<Texture>& ModelComponent::GetTexture() {
    return mTextures;
}
MeshData& ModelComponent::GetMeshData() {
    return mMeshData;
}