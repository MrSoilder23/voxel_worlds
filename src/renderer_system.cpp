#include "renderer_system.hpp"
#include <iostream>

RendererSystem::~RendererSystem() {
    std::cout << "RendererSystem bye bye" << std::endl;
}

void RendererSystem::DrawAll() {
    EntityManager& entityManager = EntityManager::GetInstance();
    for(auto it = entityManager.GetEntities().begin(); it != entityManager.GetEntities().end(); ++it) {
        auto componentPointer = entityManager.GetComponent<ModelComponent>(it->first);

        if(componentPointer) { 
            glUseProgram(mGraphicsApp->mGraphicsPipeline);

            GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
            glUniformMatrix4fv(uModelMatrixLocation, 1, false, &componentPointer->GetTransform().mModelMatrix[0][0]);

            glm::mat4 view = mGraphicsApp->mCamera.GetViewMatrix();
            GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
            glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);

            glm::mat4 perspective = mGraphicsApp->mCamera.GetProjectionMatrix();
            GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
            glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);

            componentPointer->GetMeshData().Bind();

            glDrawElements(GL_TRIANGLES, componentPointer->GetModel()->indexBufferData.size(), GL_UNSIGNED_INT, (void*)0);
        } else {
            std::cerr << "No ModelComponent" << std::endl;
        }
    }
}

void RendererSystem::AddGraphicsApp(std::shared_ptr<GraphicsApp> graphicsApp) {
    mGraphicsApp = graphicsApp;
}

RendererSystem& RendererSystem::GetInstance() {
    static RendererSystem sInstance;
    return sInstance;
}