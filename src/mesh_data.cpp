#include "mesh_data.hpp"
#include <iostream>

MeshData::MeshData() : mVertexArrayObject(0),
            mVertexBufferObject(0), mVertexBufferObjectColor(0), mVertexBufferObjectPosition(0),
            mIndexBufferObject(0) {

}
MeshData::~MeshData() {
    glDeleteBuffers(1, &mVertexArrayObject);
    glDeleteBuffers(1, &mVertexBufferObjectColor);
    glDeleteBuffers(1, &mIndexBufferObject);

    glDeleteVertexArrays(1, &mVertexArrayObject);
}

void MeshData::Initialize(const std::shared_ptr<Model>& model, const std::shared_ptr<Texture>& texture) {
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, model->vertexPositions.size()*sizeof(GLfloat), model->vertexPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &mIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indexBufferData.size()*sizeof(GLuint), model->indexBufferData.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}

void MeshData::Bind() {
    glBindVertexArray(mVertexArrayObject);
}
void MeshData::UnBind() {
    glBindVertexArray(0);
}

void MeshData::BindVBOPosition() {
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObjectPosition);
}
