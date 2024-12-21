#include "mesh_data.hpp"

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

void MeshData::Initialize(Model& model, Texture& texture) {
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, model.vertexPositions.size()*sizeof(GLfloat), model.vertexPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &mIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indexBufferData.size()*sizeof(GLuint), model.indexBufferData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mVertexBufferObjectColor);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObjectColor);
    glBufferData(GL_ARRAY_BUFFER, texture.colorPositions.size()*sizeof(GLfloat), texture.colorPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
void MeshData::Initialize(Model& model, Texture& texture, glm::vec3& position) {
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, model.vertexPositions.size()*sizeof(GLfloat), model.vertexPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &mIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indexBufferData.size()*sizeof(GLuint), model.indexBufferData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mVertexBufferObjectColor);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObjectColor);
    glBufferData(GL_ARRAY_BUFFER, texture.colorPositions.size()*sizeof(GLfloat), texture.colorPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &mVertexBufferObjectPosition);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObjectPosition);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), glm::value_ptr(position), GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void MeshData::Bind() {
    glBindVertexArray(mVertexArrayObject);
}
void MeshData::UnBind() {
    glBindVertexArray(0);
}
