#pragma once
// C++ standard libraries
#include <vector>
// Third_party libraries
#include <glm/glm.hpp>
// Own libraries
#include "./core/component.hpp"
#include "./model.hpp"
#include "./graphics/texture.hpp"

struct ChunkModelComponent : public IComponent {
    std::vector<glm::vec3> mTexturePositions;
    std::vector<GLuint> mTextures;
    
    Model mModel;
    
    GLuint mVAO = 0;
    GLuint mVBO = 0;
    GLuint mEBO = 0;
    GLuint mTexId = 0;
    GLuint mTexBO = 0;
    GLuint mTexCoords = 0;

    size_t mChangeSize = 0;
    bool mGenerated = false;

    ~ChunkModelComponent() {
        if(mVAO != 0) {
            glDeleteVertexArrays(1, &mVAO);
            glDeleteBuffers(1, &mVBO);
            glDeleteBuffers(1, &mEBO);
            glDeleteBuffers(1, &mTexId);
            glDeleteBuffers(1, &mTexBO);
            glDeleteBuffers(1, &mTexCoords);
        }
    }
};