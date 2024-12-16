#pragma once
// Third_party libraries
#include <glad/glad.h>

// Own libraries
#include "camera.hpp"

struct GraphicsApp {
    GLuint mGraphicsPipeline = 0;
    Camera mCamera;
};