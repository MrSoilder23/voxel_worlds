#include "./systems/renderer_system.hpp"

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

RendererSystem::RendererSystem(std::shared_ptr<GraphicsApp> graphicsApp) : mGraphicsApp(graphicsApp) {
    mModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
    mViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
    mProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
}

RendererSystem::~RendererSystem() {
    std::cout << "RendererSystem bye bye" << std::endl;
}

void RendererSystem::update(bismuth::Registry& registry) {
    auto cameraView = registry.getView<CameraComponent>();
    if (cameraView.begin() == cameraView.end()) {
        return;
    }

    auto [cameraEntity, camera] = *cameraView.begin();
    
    auto view = registry.getView<MeshComponent, MaterialComponent, PositionComponent>();
    auto& boundingPool = registry.getComponentPool<BoundingBoxComponent>();

    glUseProgram(mGraphicsApp->mGraphicsPipeline);

    const glm::mat4& perspective = camera.projectionMatrix;
    glUniformMatrix4fv(mViewLocation, 1, false, &camera.viewMatrix[0][0]);
    glUniformMatrix4fv(mProjectionLocation, 1, false, &perspective[0][0]);

    for(auto [entity, mesh, material, position] : view) {
        if (mesh.vertices.empty()) {
            continue;
        }

        if (registry.hasComponent<BoundingBoxComponent>(entity)) {
            auto& boundingBox = boundingPool.getComponent(entity);
            if (!physics::IsAABBInFrustum(boundingBox, camera.frustumPlanes)) {
                continue;
            }
        }

        if (mesh.VAO != 0) {
            glUniformMatrix4fv(mModelMatrixLocation, 1, false, &position.transform[0][0]);
            
            if (material.textureAtlas && material.textureAtlas->id != 0) {
                glBindTexture(GL_TEXTURE_2D, material.textureAtlas->id);
            } else {
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            
            glBindVertexArray(mesh.VAO);
            glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, (void*)0);
        }
    }
}
