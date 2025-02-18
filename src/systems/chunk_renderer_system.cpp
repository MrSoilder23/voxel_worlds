#include "./systems/chunk_renderer_system.hpp"

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


void ChunkRendererSystem::DrawChunk(std::shared_ptr<Chunk>& chunk, EntityManager& entityManager) {

    if(!chunk) {
        return;
    }

    if(chunk->mVertexArrayObject == 0 || chunk->mModel.indexBufferData.size() == 0 || !chunk->wasGenerated) {
        return;
    }

    auto cameraComponent = entityManager.GetComponent<CameraComponent>("Player");
    auto positionComponent = entityManager.GetComponent<PositionComponent>("Player");
    
    GLCheck(glUseProgram(mGraphicsApp->mGraphicsPipeline);)

    GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
    GLCheck(glUniformMatrix4fv(uModelMatrixLocation, 1, false, &chunk->mTransform.mModelMatrix[0][0]);)

    glm::mat4 view = glm::lookAt(cameraComponent->mEye, cameraComponent->mEye + positionComponent->mViewDirection, cameraComponent->mUpVector);
    GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
    GLCheck(glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);)

    glm::mat4 perspective = cameraComponent->mProjectionMatrix;
    GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
    GLCheck(glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);)
    
    GLCheck(glBindVertexArray(chunk->mVertexArrayObject);)
    GLCheck(glDrawElements(GL_TRIANGLES, chunk->mModel.indexBufferData.size(), GL_UNSIGNED_INT, (void*)0);)
    GLCheck(glBindVertexArray(0);)
}

void ChunkRendererSystem::AddGraphicsApp(std::shared_ptr<GraphicsApp>& graphicsApp) {
    if (!glIsProgram(graphicsApp->mGraphicsPipeline)) {
        std::cerr << "The shader program is not a valid OpenGL program!" << std::endl;
        exit(1);
    }

    mGraphicsApp = graphicsApp;
}

ChunkRendererSystem& ChunkRendererSystem::GetInstance() {
    static ChunkRendererSystem sInstance;
    return sInstance;
}