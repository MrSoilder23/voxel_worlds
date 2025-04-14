#pragma once
// C++ standard libraries
#include <memory>

// Own libraries
#include "./graphics/texture.hpp"
#include "./world/transform.hpp"
#include "./core/component.hpp"
#include "./model.hpp"

struct ModelComponent : public IComponent {
    GLuint mTexture;
    Model mModel;
    
    GLuint mVAO = 0;
    GLuint mVBO = 0;
    GLuint mEBO = 0;

    virtual ~ModelComponent() {
        if(mVAO != 0) {
            glDeleteVertexArrays(1, &mVAO);
            glDeleteBuffers(1, &mVBO);
            glDeleteBuffers(1, &mEBO);
        }
    }
};