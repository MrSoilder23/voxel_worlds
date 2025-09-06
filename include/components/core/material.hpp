#pragma once
// C++ standard libraries
#include <memory>

// Third_party libraries
#include <glad/glad.h>

struct Texture {
    GLuint id = 0;
    int width = 0;
    int height = 0;
    int channels = 0;
    GLenum format = GL_RGBA;
};

struct MaterialComponent {
    std::shared_ptr<Texture> textureAtlas;
};