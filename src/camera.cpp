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

void Camera::MouseLook(float mouseDeltaX, float mouseDeltaY, float sensitivity, float deltaTime) {
    float yaw = -glm::radians(mouseDeltaX * sensitivity * deltaTime);
    float pitch = -glm::radians(mouseDeltaY * sensitivity * deltaTime);

    glm::quat yawRotation = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::quat pitchRotation = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));

    mOrientation = yawRotation * mOrientation;
    mOrientation = mOrientation * pitchRotation;

    mOrientation = glm::normalize(mOrientation);

    mViewDirection = glm::rotate(mOrientation, glm::vec3(0.0f, 0.0f, -1.0f));
}

void Camera::MoveForward(float speed, float deltaTime) {
    mEye += mViewDirection * speed * deltaTime;
}
void Camera::MoveBackward(float speed, float deltaTime) {
    mEye -= mViewDirection * speed * deltaTime;
}
void Camera::MoveLeft(float speed, float deltaTime) {
    glm::vec3 rightVector = glm::cross(mViewDirection, mUpVector);
    rightVector = glm::normalize(rightVector);
    mEye -= rightVector * speed * deltaTime;
}
void Camera::MoveRight(float speed, float deltaTime) {
    glm::vec3 rightVector = glm::cross(mViewDirection, mUpVector);
    rightVector = glm::normalize(rightVector);
    mEye += rightVector * speed * deltaTime;
}