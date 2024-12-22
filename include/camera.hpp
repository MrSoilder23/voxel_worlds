#pragma once
// Third_party libraries
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera {
    public:
        Camera();

        void SetProjectionMatrix(float fov, float aspect, float near);

        glm::mat4 GetProjectionMatrix() const;
        
        glm::mat4 GetViewMatrix() const;

        void MouseLook(float mouseDeltaX, float mouseDeltaY,float sensitivity, float deltaTime);

        void MoveForward(float speed, float deltaTime);
        void MoveBackward(float speed, float deltaTime);

        void MoveLeft(float speed, float deltaTime);
        void MoveRight(float speed, float deltaTime);
        
        void MoveUp(float speed, float deltaTime);
        void MoveDown(float speed, float deltaTime);

    private:
        glm::mat4 mProjectionMatrix;
    
        glm::quat mOrientation;

        glm::vec3 mEye;
        glm::vec3 mViewDirection;
        glm::vec3 mUpVector;
};