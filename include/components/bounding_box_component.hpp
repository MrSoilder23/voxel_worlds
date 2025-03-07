#pragma once
// Third_party libraries
#include <glm/glm.hpp>
// Own libraries
#include "./core/component.hpp"
#include "./model.hpp"
#include "./group.hpp"

struct BoundingBoxComponent : public IComponent{
    // Corners, with minimum value and maximum value
    glm::vec3 mLocalMin;
    glm::vec3 mLocalMax;
    
    glm::vec3 mWorldMin;
    glm::vec3 mWorldMax;

    Group group;
    Group mask;

    // Model
    Model mModel;
    
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
};