#include "./utility/utility.hpp"

void utility::GetOpenGlVersionInfo() {
    std::cout << "Vendor: "   <<         glGetString(GL_VENDOR)           << std::endl;
    std::cout << "Renderer: " <<         glGetString(GL_RENDERER)         << std::endl;
    std::cout << "Version: "  <<         glGetString(GL_VERSION)          << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

// Transform
void utility::MeshTranslate(Transform& transform, float x, float y, float z) {
    transform.mModelMatrix = glm::translate(transform.mModelMatrix, glm::vec3(x,y,z));
}
void utility::MeshTranslate(Transform& transform, glm::vec3 transformation) {
    transform.mModelMatrix = glm::translate(transform.mModelMatrix, transformation);
}

void utility::MeshRotate(Transform& transform, float angle, glm::vec3 axis) {
    transform.mModelMatrix = glm::rotate(transform.mModelMatrix, angle, axis);
}

void utility::MeshScale(Transform& transform, float x, float y, float z) {
    transform.mModelMatrix = glm::scale(transform.mModelMatrix, glm::vec3(x,y,z));
}

// Position Component tools
void utility::MovePosition(PositionComponent& positionComponent, const glm::vec3& newPosition) {
    positionComponent.mPosition = newPosition;
    positionComponent.mDirty = true;
}

void utility::RotatePosition(PositionComponent& positionComponent, const glm::quat& newRotation) {
    positionComponent.mRotation = newRotation;
    positionComponent.mDirty = true;
}

void utility::ScalePosition(PositionComponent& positionComponent, const glm::vec3& newScale) {
    positionComponent.mScale = newScale;
    positionComponent.mDirty = true;
}

float utility::Smooth(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

inline char* utility::FastIntToString(char* ptr, int value) {
    if (value == 0) {
        *ptr++ = '0';
        return ptr;
    }

    const bool negative = value < 0;
    if (negative) {
        *ptr++ = '-';
        value = -value;
    }

    char buffer[16];
    int i = 0;
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i > 0) {
        *ptr++ = buffer[--i];
    }

    return ptr;
}

void utility::FastChunkName(char* ptr, int chunkX, int chunkY, int chunkZ) {
    ptr = FastIntToString(ptr, chunkX);
    *ptr++ = ':';
    ptr = FastIntToString(ptr, chunkY);
    *ptr++ = ':';
    ptr = FastIntToString(ptr, chunkZ);
    *ptr++ = '\0';
}