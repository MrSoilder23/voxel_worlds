#include "chunk_renderer_system.hpp"

// Error handling
static void GLClearAllErrors() {
    while(glGetError() != GL_NO_ERROR) {

    }
}

static bool GLCheckErrorStatus(const char* function, int line) {
    while(GLenum error = glGetError()) {
        std::cout << "OpenGL error: " << error << "\tLine: " << line << "\tfunction: " << function << std::endl;
        return true;
    }
    return false;
}

#define GLCheck(x) GLClearAllErrors(); x; GLCheckErrorStatus(#x, __LINE__);


void ChunkRendererSystem::DrawChunk(std::shared_ptr<Chunk>& chunk) {
    
    std::vector<glm::mat4> modelMatrices;
    for(auto it = chunk->blocks.begin(); it != chunk->blocks.end(); ++it) {
        auto modelComponentLocation = it->find(std::type_index(typeid(ModelComponent)));
        if (modelComponentLocation == it->end()) {
            continue;
        }
        auto modelComponent = std::dynamic_pointer_cast<ModelComponent>(modelComponentLocation->second);

        modelMatrices.push_back(modelComponent->GetTransform().mModelMatrix);
    }


    GLCheck(glUseProgram(mGraphicsApp->mGraphicsPipeline));

    auto modelComponent = std::dynamic_pointer_cast<ModelComponent>(chunk->blocks[0][std::type_index(typeid(ModelComponent))]);
    GLCheck(modelComponent->GetMeshData().Bind();)

    GLuint instanceBuffer;
    glGenBuffers(1, &instanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), modelMatrices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(2, 1); 
    glVertexAttribDivisor(3, 1); 
    glVertexAttribDivisor(4, 1); 
    glVertexAttribDivisor(5, 1); 

    // GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
    // glUniformMatrix4fv(uModelMatrixLocation, 1, false, &modelMatrices);

    glm::mat4 view = mGraphicsApp->mCamera.GetViewMatrix();
    GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
    GLCheck(glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]));

    glm::mat4 perspective = mGraphicsApp->mCamera.GetProjectionMatrix();
    GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
    GLCheck(glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]));

    

    GLCheck(glDrawElementsInstanced(GL_TRIANGLES, modelComponent->GetModel()->indexBufferData.size(), GL_UNSIGNED_INT, (void*)0, chunk->blocks.size()));

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);

    GLCheck(modelComponent->GetMeshData().UnBind();)
}

void ChunkRendererSystem::AddGraphicsApp(std::shared_ptr<GraphicsApp>& graphicsApp) {
    mGraphicsApp = graphicsApp;
}

ChunkRendererSystem& ChunkRendererSystem::GetInstance() {
    static ChunkRendererSystem sInstance;
    return sInstance;
}