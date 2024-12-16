#pragma once
// C++ standard libraries
#include <iostream>
#include <memory>

// Third_Party libraries
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Own Libraries
#include "mesh_data.hpp"

namespace utility {
    
    inline void GetOpenGlVersionInfo() {
        std::cout << "Vendor: "   <<         glGetString(GL_VENDOR)           << std::endl;
        std::cout << "Renderer: " <<         glGetString(GL_RENDERER)         << std::endl;
        std::cout << "Version: "  <<         glGetString(GL_VERSION)          << std::endl;
        std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    }

    inline void MeshTranslate(MeshData& mesh, float x, float y, float z) {
        mesh.mTransform.mModelMatrix = glm::translate(mesh.mTransform.mModelMatrix, glm::vec3(x,y,z));
    }
    inline void MeshRotate(MeshData& mesh, float angle, glm::vec3 axis) {
        mesh.mTransform.mModelMatrix = glm::rotate(mesh.mTransform.mModelMatrix, angle, axis);
    }
    inline void MeshScale(MeshData& mesh, float x, float y, float z) {
        mesh.mTransform.mModelMatrix = glm::scale(mesh.mTransform.mModelMatrix, glm::vec3(x,y,z));
    }
}
