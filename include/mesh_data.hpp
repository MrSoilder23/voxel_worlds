#pragma once
// C++ standard libraries
#include <memory>

// Third_party libraries
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Own libraries
#include "model.hpp"
#include "texture.hpp"

class MeshData {
    public:
        MeshData();
        ~MeshData();

        void Initialize(const std::shared_ptr<Model>& model, const std::shared_ptr<Texture>& texture);
        void Initialize(const std::shared_ptr<Model>& model, const std::shared_ptr<Texture>& texture, GLfloat position[]);

        void Bind();
        void UnBind();

        void BindVBOPosition();

    private:
        GLuint mVertexArrayObject = 0;

        GLuint mVertexBufferObject = 0;
        GLuint mVertexBufferObjectColor = 0;
        GLuint mVertexBufferObjectPosition = 0;

        GLuint mIndexBufferObject = 0;
};