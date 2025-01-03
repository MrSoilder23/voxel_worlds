#include "model_component.hpp"
#include <iostream>

ModelComponent::ModelComponent() {
}

void ModelComponent::Initialize() {
}

void ModelComponent::AddModel(const Model& model) {
    mModel = model;
}
void ModelComponent::AddTextures(const std::shared_ptr<Texture>& textures) {
    mTextures = textures;
}

Model& ModelComponent::GetModel() {
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