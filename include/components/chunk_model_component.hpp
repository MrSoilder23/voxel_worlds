#pragma once
// C++ standard libraries
#include <vector>
// Third_party libraries
#include <glm/glm.hpp>
// Own libraries
#include "./components/model_component.hpp"

struct ChunkModelComponent : public ModelComponent {
    std::vector<glm::vec3> mTexturePositions;
    std::vector<GLuint> mTextures;
    
    GLuint mTexId = 0;
    GLuint mTexBO = 0;
    GLuint mTexCoords = 0;

    bool mGenerated = false;

    ~ChunkModelComponent() override {
        if(mVAO != 0) {
            glDeleteVertexArrays(1, &mVAO);
            glDeleteBuffers(1, &mVBO);
            glDeleteBuffers(1, &mEBO);
            glDeleteBuffers(1, &mTexId);
            glDeleteBuffers(1, &mTexCoords);
        }
    }
};