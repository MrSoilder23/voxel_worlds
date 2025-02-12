#pragma once
// C++ standard libraries
#include <memory>

// Own libraries
#include "./world/transform.hpp"
#include "./core/component.hpp"
#include "model.hpp"
#include "texture.hpp"
#include "shader.hpp"

class ModelComponent : public IComponent {
    public: 
        std::shared_ptr<Texture> mTextures;
        Transform mTransform;
        Model mModel;

};