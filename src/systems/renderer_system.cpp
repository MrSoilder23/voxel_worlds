#include "./systems/renderer_system.hpp"
#include <iostream>
#include "./utility/utility.hpp"
#include "./components/camera_component.hpp"
#include "./components/position_component.hpp"

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

RendererSystem::~RendererSystem() {
    std::cout << "RendererSystem bye bye" << std::endl;
}

void RendererSystem::DrawAll() {
    EntityManager& entityManager = EntityManager::GetInstance();
    auto cameraComponent = entityManager.GetComponent<CameraComponent>("Player");
    auto positionComponent = entityManager.GetComponent<PositionComponent>("Player");
    
    for(auto it = entityManager.GetEntities().begin(); it != entityManager.GetEntities().end(); ++it) {
        auto componentPointer = entityManager.GetComponent<ModelComponent>(it->first);

        if(componentPointer) { 
            glUseProgram(mGraphicsApp->mGraphicsPipeline);

            GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
            glUniformMatrix4fv(uModelMatrixLocation, 1, false, &componentPointer->mTransform.mModelMatrix[0][0]);

            glm::mat4 view = glm::lookAt(cameraComponent->mEye, cameraComponent->mEye + positionComponent->mViewDirection, cameraComponent->mUpVector);
            GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
            glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);

            glm::mat4 perspective = cameraComponent->mProjectionMatrix;
            GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
            glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);

            // componentPointer->GetMeshData().Bind();
            glBindVertexArray(componentPointer->VAO);
            GLCheck(glDrawElements(GL_LINES, static_cast<GLsizei>(componentPointer->mModel.indexBufferData.size()), GL_UNSIGNED_INT, (void*)0);)
        } else {
            // std::cerr << "No ModelComponent" << std::endl;
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