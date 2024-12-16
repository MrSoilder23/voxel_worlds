#pragma once
// C++ standard libraries
#include <memory>

// Own libraries
#include "component.hpp"
#include "model.hpp"
#include "texture.hpp"
#include "mesh_data.hpp"
#include "shader.hpp"

class ModelComponent : public IComponent {
    public: 
        ModelComponent();
        ~ModelComponent();

        void Initialize() override;
        
        void AddModel(const std::shared_ptr<Model>& model);
        void AddTextures(const std::shared_ptr<Texture>& textures);

        std::shared_ptr<Model>& GetModel();
        std::shared_ptr<Texture>& GetTexture();
        MeshData& GetMeshData();

    private:
        std::shared_ptr<Model> mModel;
        std::shared_ptr<Texture> mTextures;
        MeshData mMeshData;

};