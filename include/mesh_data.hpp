#pragma once
// C++ standard libraries
#include <memory>

// Third_party libraries
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Own libraries
#include "model.hpp"
#include "./graphics/texture.hpp"
#include "./graphics/shader.hpp"

class MeshData {
    public:
        MeshData();
        ~MeshData();

        void Initialize(const Model& model);

        void Bind();
        void UnBind();

        GLuint& GetVAO();

        void BindVBOPosition();

    private:
        GLuint mVertexArrayObject = 0;

        GLuint mVertexBufferObject = 0;
        GLuint mVertexBufferObjectColor = 0;
        GLuint mVertexBufferObjectPosition = 0;

        GLuint mIndexBufferObject = 0;
};