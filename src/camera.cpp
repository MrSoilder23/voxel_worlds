#include "camera.hpp"

Camera::Camera() {
    mEye = glm::vec3(0.0f);
    mViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::SetProjectionMatrix(float fov, float aspect, float near) {
    mProjectionMatrix = glm::infinitePerspective(fov, aspect, near);
}

glm::mat4 Camera::GetProjectionMatrix() const{
    return mProjectionMatrix;
}

glm::mat4 Camera::GetViewMatrix() const{
    return glm::lookAt(mEye, mEye + mViewDirection, mUpVector);
}