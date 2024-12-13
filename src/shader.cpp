#include "shader.hpp"
#include <iostream>

namespace {
    std::string LoadShaderAsString(const std::string& src) {
        std::string result = "";

        std::string line = "";
        std::ifstream myFile(src.c_str());

        if(myFile.is_open()) {
            while(std::getline(myFile, line)) {
                result += line + "\n";
            }
            myFile.close();
        }

        return result;
    }

    GLuint CompileShader(GLuint type, const std::string& source) {
        GLuint shaderObject;

        shaderObject = glCreateShader(type);

        const char* src = source.c_str();
        glShaderSource(shaderObject, 1, &src, nullptr);
        glCompileShader(shaderObject);

        return shaderObject;
        
    }

    GLuint CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
        GLuint programObject = glCreateProgram();

        GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
        GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        glAttachShader(programObject ,myVertexShader);
        glAttachShader(programObject ,myFragmentShader);
        glLinkProgram(programObject);

        glValidateProgram(programObject);

        return programObject;
    }

}

void shader::CreateGraphicsPipeline(GLuint& shaderProgram, const std::string& _vertexShaderSource, const std::string& _fragmentShaderSource) {
    std::string vertexShaderSource = LoadShaderAsString(vertexShaderSource);
    std::string fragmentShaderSource = LoadShaderAsString(fragmentShaderSource);

    shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}