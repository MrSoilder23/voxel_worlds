#pragma once
// C++ standard libraries
#include <iostream>
#include <memory>

// Third_Party libraries
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Own Libraries
#include "transform.hpp"

namespace utility {
    
    inline void GetOpenGlVersionInfo() {
        std::cout << "Vendor: "   <<         glGetString(GL_VENDOR)           << std::endl;
        std::cout << "Renderer: " <<         glGetString(GL_RENDERER)         << std::endl;
        std::cout << "Version: "  <<         glGetString(GL_VERSION)          << std::endl;
        std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    }

    inline void MeshTranslate(Transform& transform, float x, float y, float z) {
        transform.mModelMatrix = glm::translate(transform.mModelMatrix, glm::vec3(x,y,z));
    }
    inline void MeshTranslate(Transform& transform, glm::vec3 transformation) {
        transform.mModelMatrix = glm::translate(transform.mModelMatrix, transformation);
    }

    inline void MeshRotate(Transform& transform, float angle, glm::vec3 axis) {
        transform.mModelMatrix = glm::rotate(transform.mModelMatrix, angle, axis);
    }

    inline void MeshScale(Transform& transform, float x, float y, float z) {
        transform.mModelMatrix = glm::scale(transform.mModelMatrix, glm::vec3(x,y,z));
    }
}
