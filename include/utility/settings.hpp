#pragma once
// Third_party libraries
#include <glad/glad.h>

struct Settings {
    GLuint mGraphicsShaderProgram = 0;

    int screenWidth  = 1366;
    int screenHeight = 768;

    float sensitivity = 0.25f;

    float speed        = 100.0f;    
    bool boundingDebug = false;
    bool physics       = true;
    bool worldGen      = true;
};