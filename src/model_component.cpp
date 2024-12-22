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

void ModelComponent::Initialize() {
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
Transform& ModelComponent::GetTransform() {
    return mTransform;
}