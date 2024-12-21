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
    
    for(auto it = chunk->blocks.begin(); it != chunk->blocks.end(); ++it) {
        auto modelComponentLocation = it->find(std::type_index(typeid(ModelComponent)));

        if (modelComponentLocation == it->end()) {
            continue;
        }

        auto modelComponent = std::dynamic_pointer_cast<ModelComponent>(modelComponentLocation->second);

        GLCheck(glUseProgram(mGraphicsApp->mGraphicsPipeline));

        GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
        GLCheck(glUniformMatrix4fv(uModelMatrixLocation, 1, false, &modelComponent->GetTransform().mModelMatrix[0][0]));

        glm::mat4 view = mGraphicsApp->mCamera.GetViewMatrix();
        GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
        GLCheck(glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]));

        glm::mat4 perspective = mGraphicsApp->mCamera.GetProjectionMatrix();
        GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
        GLCheck(glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]));

        modelComponent->GetMeshData().Bind();

        GLCheck(glDrawElementsInstanced(GL_TRIANGLES, modelComponent->GetModel()->indexBufferData.size(), GL_UNSIGNED_INT, (void*)0, 32'768));
    }
}

void ChunkRendererSystem::AddGraphicsApp(std::shared_ptr<GraphicsApp>& graphicsApp) {
    mGraphicsApp = graphicsApp;
}

ChunkRendererSystem& ChunkRendererSystem::GetInstance() {
    static ChunkRendererSystem sInstance;
    return sInstance;
}