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

void RendererSystem::AddGraphicsApp(std::shared_ptr<GraphicsApp> graphicsApp) {
    mGraphicsApp = graphicsApp;
}
void RendererSystem::DrawAll(EntityManager& entityManager) {
    auto cameraComponent = entityManager.GetComponent<CameraComponent>("Player");
    auto positionComponent = entityManager.GetComponent<PositionComponent>("Player");
    
    for(const auto& entityPointer : entityManager.GetEntities()) {
        auto modelPositionComponent = entityManager.GetComponent<PositionComponent>(entityPointer.first);
        auto chunkModelComponent = entityManager.GetComponent<ChunkModelComponent>(entityPointer.first);

        if(chunkModelComponent && chunkModelComponent->mVAO != 0) {
            glUseProgram(mGraphicsApp->mGraphicsPipeline);

            GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
            glUniformMatrix4fv(uModelMatrixLocation, 1, false, &modelPositionComponent->mTransform[0][0]);

            glm::mat4 view = glm::lookAt(positionComponent->mPosition, positionComponent->mPosition + cameraComponent->mViewDirection, cameraComponent->mUpVector);
            GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
            glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);

            glm::mat4 perspective = cameraComponent->mProjectionMatrix;
            GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
            glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);

            glBindVertexArray(chunkModelComponent->mVAO);
            GLCheck(glDrawElements(GL_TRIANGLES, chunkModelComponent->mModel.indexBufferData.size(), GL_UNSIGNED_INT, (void*)0);)

        } else {
            auto modelComponent = entityManager.GetComponent<ModelComponent>(entityPointer.first);
            
            if(modelComponent && modelComponent->VAO != 0) { 
                glUseProgram(mGraphicsApp->mGraphicsPipeline);
        
                GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
                glUniformMatrix4fv(uModelMatrixLocation, 1, false, &modelPositionComponent->mTransform[0][0]);
        
                glm::mat4 view = glm::lookAt(positionComponent->mPosition, positionComponent->mPosition + cameraComponent->mViewDirection, cameraComponent->mUpVector);
                GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
                glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);
        
                glm::mat4 perspective = cameraComponent->mProjectionMatrix;
                GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
                glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);
        
                glBindVertexArray(modelComponent->VAO);
                GLCheck(glDrawElements(GL_TRIANGLES, modelComponent->mModel.indexBufferData.size(), GL_UNSIGNED_INT, (void*)0);)
            }
        }
    }
}
void RendererSystem::DrawAllSingle(EntityManager& entityManager, std::string entityName) {
    static auto cameraComponent = entityManager.GetComponent<CameraComponent>("Player");
    static auto positionComponent = entityManager.GetComponent<PositionComponent>("Player");
    
    auto modelPositionComponent = entityManager.GetComponent<PositionComponent>(entityName);
    auto chunkModelComponent = entityManager.GetComponent<ChunkModelComponent>(entityName);

    if(chunkModelComponent && chunkModelComponent->mVAO != 0) {
        glUseProgram(mGraphicsApp->mGraphicsPipeline);

        GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
        glUniformMatrix4fv(uModelMatrixLocation, 1, false, &modelPositionComponent->mTransform[0][0]);

        glm::mat4 view = glm::lookAt(positionComponent->mPosition, positionComponent->mPosition + cameraComponent->mViewDirection, cameraComponent->mUpVector);
        GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
        glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);

        glm::mat4 perspective = cameraComponent->mProjectionMatrix;
        GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
        glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);

        glBindVertexArray(chunkModelComponent->mVAO);
        GLCheck(glDrawElements(GL_TRIANGLES, chunkModelComponent->mModel.indexBufferData.size(), GL_UNSIGNED_INT, (void*)0);)

    } else {
        auto modelComponent = entityManager.GetComponent<ModelComponent>(entityName);

        if(modelComponent && modelComponent->VAO != 0) { 
            glUseProgram(mGraphicsApp->mGraphicsPipeline);
    
            GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
            glUniformMatrix4fv(uModelMatrixLocation, 1, false, &modelPositionComponent->mTransform[0][0]);
    
            glm::mat4 view = glm::lookAt(positionComponent->mPosition, positionComponent->mPosition + cameraComponent->mViewDirection, cameraComponent->mUpVector);
            GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
            glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);
    
            glm::mat4 perspective = cameraComponent->mProjectionMatrix;
            GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
            glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);
    
            glBindVertexArray(modelComponent->VAO);
            GLCheck(glDrawElements(GL_TRIANGLES, modelComponent->mModel.indexBufferData.size(), GL_UNSIGNED_INT, (void*)0);)
        }
    }
}


void RendererSystem::DrawAllDebug(EntityManager& entityManager) {
    auto cameraComponent = entityManager.GetComponent<CameraComponent>("Player");
    auto positionComponent = entityManager.GetComponent<PositionComponent>("Player");
    
    for(const auto& entityPointer : entityManager.GetEntities()) {
        auto boundingBoxComp = entityManager.GetComponent<BoundingBoxComponent>(entityPointer.first);
        auto boundingPos = entityManager.GetComponent<PositionComponent>(entityPointer.first);

        if(boundingBoxComp) { 
            // Reset Rotations for bounding boxes
            boundingPos->mTransform[0][0] = 1.0f; boundingPos->mTransform[0][1] = 0.0f; boundingPos->mTransform[0][2] = 0.0f;
            boundingPos->mTransform[1][0] = 0.0f; boundingPos->mTransform[1][1] = 1.0f; boundingPos->mTransform[1][2] = 0.0f;
            boundingPos->mTransform[2][0] = 0.0f; boundingPos->mTransform[2][1] = 0.0f; boundingPos->mTransform[2][2] = 1.0f;

            glUseProgram(mGraphicsApp->mGraphicsPipeline);

            GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
            glUniformMatrix4fv(uModelMatrixLocation, 1, false, &boundingPos->mTransform[0][0]);

            glm::mat4 view = glm::lookAt(positionComponent->mPosition, positionComponent->mPosition + cameraComponent->mViewDirection, cameraComponent->mUpVector);
            GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
            glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);

            glm::mat4 perspective = cameraComponent->mProjectionMatrix;
            GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
            glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);

            // componentPointer->GetMeshData().Bind();
            glBindVertexArray(boundingBoxComp->VAO);
            glDrawElements(GL_LINES, static_cast<GLsizei>(boundingBoxComp->mModel.indexBufferData.size()), GL_UNSIGNED_INT, (void*)0);
        } else {
            // std::cerr << "No ModelComponent" << std::endl;
        }
    }
}

void RendererSystem::DrawAllDebugSingle(EntityManager& entityManager, std::string entityName) {
    static auto cameraComponent = entityManager.GetComponent<CameraComponent>("Player");
    static auto positionComponent = entityManager.GetComponent<PositionComponent>("Player");

    auto boundingBoxComp = entityManager.GetComponent<BoundingBoxComponent>(entityName);
    auto boundingPos = entityManager.GetComponent<PositionComponent>(entityName);

    if(boundingBoxComp && boundingBoxComp->VAO != 0) { 
        // Reset Rotations for bounding boxes
        boundingPos->mTransform[0][0] = 1.0f; boundingPos->mTransform[0][1] = 0.0f; boundingPos->mTransform[0][2] = 0.0f;
        boundingPos->mTransform[1][0] = 0.0f; boundingPos->mTransform[1][1] = 1.0f; boundingPos->mTransform[1][2] = 0.0f;
        boundingPos->mTransform[2][0] = 0.0f; boundingPos->mTransform[2][1] = 0.0f; boundingPos->mTransform[2][2] = 1.0f;

        glUseProgram(mGraphicsApp->mGraphicsPipeline);

        GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
        glUniformMatrix4fv(uModelMatrixLocation, 1, false, &boundingPos->mTransform[0][0]);

        glm::mat4 view = glm::lookAt(positionComponent->mPosition, positionComponent->mPosition + cameraComponent->mViewDirection, cameraComponent->mUpVector);
        GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
        glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);

        glm::mat4 perspective = cameraComponent->mProjectionMatrix;
        GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
        glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);

        // componentPointer->GetMeshData().Bind();
        glBindVertexArray(boundingBoxComp->VAO);
        glDrawElements(GL_LINES, static_cast<GLsizei>(boundingBoxComp->mModel.indexBufferData.size()), GL_UNSIGNED_INT, (void*)0);
    }
}

RendererSystem& RendererSystem::GetInstance() {
    static RendererSystem sInstance;
    return sInstance;
}