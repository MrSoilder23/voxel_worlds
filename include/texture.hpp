#pragma once
// C++ standard libraries
#include <vector>
#include <memory>

// Third_party libraries
#include <glad/glad.h>

struct Texture {
    inline Texture() {
    }

    GLuint64 textureHandle;
};