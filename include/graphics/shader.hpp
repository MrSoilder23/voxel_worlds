#pragma once
// C++ standard libraries
#include <iostream>
#include <string>
#include <fstream>

// Third_party libraries
#include <glad/glad.h>

namespace shader {

    void CreateGraphicsPipeline(GLuint& shaderProgram, const std::string& _vertexShaderSource, const std::string& _fragmentShaderSource);

    int FindUniformLocation(GLuint pipeline, const GLchar* name);

}