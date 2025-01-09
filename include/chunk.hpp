#pragma once
// C++ standard libraries
#include <typeindex>
#include <memory>
#include <vector>
#include <unordered_map>
#include <map>

// Third_party libraries
#include <glm/glm.hpp>

// Own libraries
#include "component.hpp"
#include "model_component.hpp"
#include "mesh_data.hpp"
#include "transform.hpp"
#include "model.hpp"
#include "texture.hpp"
#include "block_texture_creator.hpp"

using BlockData = std::unordered_map<std::type_index, std::shared_ptr<IComponent>>;

class Chunk {
    public:
        Chunk();

        void CreateVao();

        std::shared_ptr<Model>& GetModel();
        MeshData& GetMeshData();
        Transform& GetTransform();

    public:
        BlockData blocks[32][32][32]; // max size: 32'768 (32*32*32)
    private:
        std::shared_ptr<Model> mModel;
        std::map<GLuint64, GLuint> mTextures;
        
        std::vector<glm::vec3> mTexturePositions;
        std::vector<GLuint> mTextureID;

        MeshData mMeshData;
        Transform mTransform;
};