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

    mModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
    mViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
    mProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
}
void RendererSystem::DrawAll(EntityManager& entityManager) {
    const auto& cameraComponent = entityManager.GetComponent<CameraComponent>("Player");
    glUseProgram(mGraphicsApp->mGraphicsPipeline);

    const auto boundingBoxes = entityManager.GetComponentArray<BoundingBoxComponent>();
    const auto positions = entityManager.GetComponentArray<PositionComponent>();

    const auto chunkModels = entityManager.GetComponentArray<ChunkModelComponent>();
    const auto models = entityManager.GetComponentArray<ModelComponent>();

    for(size_t entityID = 0; entityID < boundingBoxes.size(); entityID++) {

        if(entityID >= boundingBoxes.size() || entityID >= positions.size() || entityID >= chunkModels.size() || entityID >= models.size()) {
            continue;
        }

        const auto& boundingBoxComponent = boundingBoxes[entityID];
        if(boundingBoxComponent && !physics::IsAABBInFrustum(*boundingBoxComponent, cameraComponent->frustumPlanes)) {
            continue;
        }

        const auto& modelPositionComponent = positions[entityID];
        const auto& chunkModelComponent = chunkModels[entityID];
        const auto& modelComponent = models[entityID];

        const ModelComponent* renderableModel = chunkModelComponent ? static_cast<ModelComponent*>(chunkModelComponent) : modelComponent;
        if(!renderableModel || renderableModel->mModel.vertexPositions.size() == 0) {
            continue;
        }

        if(renderableModel->mVAO != 0) {
            glUniformMatrix4fv(mModelMatrixLocation, 1, false, &modelPositionComponent->mTransform[0][0]);

            glUniformMatrix4fv(mViewLocation, 1, false, &cameraComponent->mViewMatrix[0][0]);

            const glm::mat4& perspective = cameraComponent->mProjectionMatrix;
            glUniformMatrix4fv(mProjectionLocation, 1, false, &perspective[0][0]);

            glBindVertexArray(renderableModel->mVAO);
            // GLCheck(glDrawElements(GL_TRIANGLES, renderableModel->mModel.indexBufferData.size(), GL_UNSIGNED_INT, (void*)0);)
            glDrawElements(GL_TRIANGLES, renderableModel->mModel.indexBufferData.size(), GL_UNSIGNED_INT, (void*)0);
        }
    }
}
void RendererSystem::DrawAllSingle(EntityManager& entityManager, std::string entityName) {
    static auto cameraComponent = entityManager.GetComponent<CameraComponent>("Player");
    
    auto modelPositionComponent = entityManager.GetComponent<PositionComponent>(entityName);
    auto chunkModelComponent = entityManager.GetComponent<ChunkModelComponent>(entityName);
    auto modelComponent = entityManager.GetComponent<ModelComponent>(entityName);

    ModelComponent* renderableModel = chunkModelComponent ? static_cast<ModelComponent*>(chunkModelComponent) : modelComponent;
    if(!renderableModel) {
        return;
    }

    if(renderableModel->mVAO != 0) {
        glUseProgram(mGraphicsApp->mGraphicsPipeline);

        glUniformMatrix4fv(mModelMatrixLocation, 1, false, &modelPositionComponent->mTransform[0][0]);

        glUniformMatrix4fv(mViewLocation, 1, false, &cameraComponent->mViewMatrix[0][0]);

        glm::mat4 perspective = cameraComponent->mProjectionMatrix;
        glUniformMatrix4fv(mProjectionLocation, 1, false, &perspective[0][0]);

        glBindVertexArray(chunkModelComponent->mVAO);
        GLCheck(glDrawElements(GL_TRIANGLES, chunkModelComponent->mModel.indexBufferData.size(), GL_UNSIGNED_INT, (void*)0);)

    }
}


void RendererSystem::DrawAllDebug(EntityManager& entityManager) {
    const auto& cameraComponent = entityManager.GetComponent<CameraComponent>("Player");
    glUseProgram(mGraphicsApp->mGraphicsPipeline);

    const auto& boundingBoxes = entityManager.GetComponentArray<BoundingBoxComponent>();
    const auto& positions = entityManager.GetComponentArray<PositionComponent>();

    for(size_t entityID = 0; entityID < boundingBoxes.size(); entityID++) {

        if(entityID >= boundingBoxes.size() || entityID >= positions.size()) {
            continue;
        }

        const auto& boundingBoxComponent = boundingBoxes[entityID];

        if(boundingBoxComponent && !physics::IsAABBInFrustum(*boundingBoxComponent, cameraComponent->frustumPlanes)) {
            continue;
        }

        auto boundingPos = positions[entityID];

        if(boundingBoxComponent && boundingBoxComponent->mVAO != 0) { 
            // Reset Rotations for bounding boxes
            boundingPos->mTransform[0][0] = 1.0f; boundingPos->mTransform[0][1] = 0.0f; boundingPos->mTransform[0][2] = 0.0f;
            boundingPos->mTransform[1][0] = 0.0f; boundingPos->mTransform[1][1] = 1.0f; boundingPos->mTransform[1][2] = 0.0f;
            boundingPos->mTransform[2][0] = 0.0f; boundingPos->mTransform[2][1] = 0.0f; boundingPos->mTransform[2][2] = 1.0f;

            glUniformMatrix4fv(mModelMatrixLocation, 1, false, &boundingPos->mTransform[0][0]);

            glUniformMatrix4fv(mViewLocation, 1, false, &cameraComponent->mViewMatrix[0][0]);

            const glm::mat4& perspective = cameraComponent->mProjectionMatrix;
            glUniformMatrix4fv(mProjectionLocation, 1, false, &perspective[0][0]);

            // componentPointer->GetMeshData().Bind();
            glBindVertexArray(boundingBoxComponent->mVAO);
            glDrawElements(GL_LINES, static_cast<GLsizei>(boundingBoxComponent->mModel.indexBufferData.size()), GL_UNSIGNED_INT, (void*)0);
        }
    }
}

void RendererSystem::DrawAllDebugSingle(EntityManager& entityManager, std::string entityName) {
    static auto cameraComponent = entityManager.GetComponent<CameraComponent>("Player");

    auto boundingBoxComp = entityManager.GetComponent<BoundingBoxComponent>(entityName);
    auto boundingPos = entityManager.GetComponent<PositionComponent>(entityName);

    if(boundingBoxComp && boundingBoxComp->mVAO != 0) { 
        // Reset Rotations for bounding boxes
        boundingPos->mTransform[0][0] = 1.0f; boundingPos->mTransform[0][1] = 0.0f; boundingPos->mTransform[0][2] = 0.0f;
        boundingPos->mTransform[1][0] = 0.0f; boundingPos->mTransform[1][1] = 1.0f; boundingPos->mTransform[1][2] = 0.0f;
        boundingPos->mTransform[2][0] = 0.0f; boundingPos->mTransform[2][1] = 0.0f; boundingPos->mTransform[2][2] = 1.0f;

        glUseProgram(mGraphicsApp->mGraphicsPipeline);

        glUniformMatrix4fv(mModelMatrixLocation, 1, false, &boundingPos->mTransform[0][0]);
        
        glUniformMatrix4fv(mViewLocation, 1, false, &cameraComponent->mViewMatrix[0][0]);

        glm::mat4 perspective = cameraComponent->mProjectionMatrix;
        glUniformMatrix4fv(mProjectionLocation, 1, false, &perspective[0][0]);

        // componentPointer->GetMeshData().Bind();
        glBindVertexArray(boundingBoxComp->mVAO);
        glDrawElements(GL_LINES, static_cast<GLsizei>(boundingBoxComp->mModel.indexBufferData.size()), GL_UNSIGNED_INT, (void*)0);
    }
}

RendererSystem& RendererSystem::GetInstance() {
    static RendererSystem sInstance;
    return sInstance;
}