#include "renderer_system.hpp"

void RendererSystem::DrawAll() {
    EntityManager entityManager = EntityManager::GetInstance();
    for(auto it = entityManager.GetEntities().begin(); it != entityManager.GetEntities().end(); ++it) {
        ModelComponent& component = entityManager.GetComponent<ModelComponent>(it->first);
        
        glUseProgram(mGraphicsApp->mGraphicsPipeline);

        glBindVertexArray(component.GetMeshData().mVertexArrayObject);

        GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
        glUniformMatrix4fv(uModelMatrixLocation, 1, false, &component.GetMeshData().mTransform.mModelMatrix[0][0]);

        glm::mat4 view = mGraphicsApp->mCamera.GetViewMatrix();
        GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
        glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);

        glm::mat4 perspective = mGraphicsApp->mCamera.GetProjectionMatrix();
        GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
        glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);

        glDrawElements(GL_TRIANGLES, component.GetMeshData().mIndexBufferObject/sizeof(GLuint), GL_UNSIGNED_INT, (void*)0);
    }
}

void RendererSystem::AddGraphicsApp(std::shared_ptr<GraphicsApp> graphicsApp) {
    mGraphicsApp = graphicsApp;
}

RendererSystem& RendererSystem::GetInstance() {
    static RendererSystem sInstance;
    return sInstance;
}