#pragma once
// Third_party libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    public:
        Camera();

        void SetProjectionMatrix(float fov, float aspect, float near);

        glm::mat4 GetProjectionMatrix() const;
        
        glm::mat4 GetViewMatrix() const;

    private:
        glm::mat4 mProjectionMatrix;

        glm::vec3 mEye;
        glm::vec3 mViewDirection;
        glm::vec3 mUpVector;
};