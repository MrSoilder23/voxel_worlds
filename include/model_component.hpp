#pragma once
// C++ standard libraries
#include <memory>

// Own libraries
#include "transform.hpp"
#include "component.hpp"
#include "model.hpp"
#include "texture.hpp"
#include "mesh_data.hpp"
#include "shader.hpp"

class ModelComponent : public IComponent {
    public: 
        Model mModel;
        std::shared_ptr<Texture> mTextures;
        MeshData mMeshData;
        Transform mTransform;

};