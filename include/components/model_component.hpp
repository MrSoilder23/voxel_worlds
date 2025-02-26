#pragma once
// C++ standard libraries
#include <memory>

// Own libraries
#include "./graphics/texture.hpp"
#include "./world/transform.hpp"
#include "./core/component.hpp"
#include "./model.hpp"

struct ModelComponent : public IComponent {
    std::shared_ptr<Texture> mTextures;
    Model mModel;
    
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};