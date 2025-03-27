// C++ standard libraries
#include <iostream>

#include <SDL2/SDL.h>

// Own libraries
#include "./components/bounding_box_component.hpp"
#include "./line.hpp"
#include "./utility/physics.hpp"

int main(int argc, char* argv[]) {
    BoundingBoxComponent bBox;
    Line line;

    // Point start inside and goes right
    bBox.mWorldMin = glm::vec3(0,0,0);
    bBox.mWorldMax = glm::vec3(1,1,1);

    line.mPosition = glm::vec3(0.5f,0.5f,0.5f);
    line.mDirection = glm::normalize(glm::vec3(1.0f,0,0));
    
    std::cout << "Test1: " << physics::LineIntersectsAABB(line, bBox) << std::endl; 

    // Point starts above and goes right
    bBox.mWorldMin = glm::vec3(0,0,0);
    bBox.mWorldMax = glm::vec3(1,1,1);

    line.mPosition = glm::vec3(0.5f,2.0f,0.5f);
    line.mDirection = glm::normalize(glm::vec3(1.0f,0,0));

    std::cout << "Test2: " << physics::LineIntersectsAABB(line, bBox) << std::endl;

    // Point starts above and goes into the cube
    bBox.mWorldMin = glm::vec3(0,0,0);
    bBox.mWorldMax = glm::vec3(1,1,1);

    line.mPosition = glm::vec3(-0.5f,2.0f,0.5f);
    line.mDirection = glm::normalize(glm::vec3(1.0f,-1.0f,0));

    std::cout << "Test3: " << physics::LineIntersectsAABB(line, bBox) << std::endl;

    // Point starts above and goes away from the cube
    bBox.mWorldMin = glm::vec3(0,0,0);
    bBox.mWorldMax = glm::vec3(1,1,1);

    line.mPosition = glm::vec3(-0.5f,2.0f,0.5f);
    line.mDirection = glm::normalize(glm::vec3(1.0f,1.0f,0));

    std::cout << "Test4: " << physics::LineIntersectsAABB(line, bBox) << std::endl;

    // Point starts inside the cube
    bBox.mWorldMin = glm::vec3(0,0,0);
    bBox.mWorldMax = glm::vec3(1,1,1);

    line.mPosition = glm::vec3(0.5f,0.5f,0.5f);
    line.mDirection = glm::vec3(0.0f,0.0f,0.0f);

    std::cout << "Test5: " << physics::LineIntersectsAABB(line, bBox) << std::endl;

    // Point starts at the edge and goes forward
    bBox.mWorldMin = glm::vec3(0,0,0);
    bBox.mWorldMax = glm::vec3(1,1,1);

    line.mPosition = glm::vec3(0.0f,0.0f,1.0f);
    line.mDirection = glm::normalize(glm::vec3(0.0f,0.0f,1.0f));

    std::cout << "Test6: " << physics::LineIntersectsAABB(line, bBox) << std::endl;

    // Point starts off the edge
    bBox.mWorldMin = glm::vec3(0,0,0);
    bBox.mWorldMax = glm::vec3(1,1,1);

    line.mPosition = glm::vec3(0.0f,0.0f,1.1f);
    line.mDirection = glm::normalize(glm::vec3(0.0f,0.0f,1.0f));

    std::cout << "Test7: " << physics::LineIntersectsAABB(line, bBox) << std::endl;

    // Point starts off the edge
    bBox.mWorldMin = glm::vec3(0,0,0);
    bBox.mWorldMax = glm::vec3(1,1,1);

    line.mPosition = glm::vec3(-100.0f,0.5f,0.5f);
    line.mDirection = glm::normalize(glm::vec3(1.0f,0.0f,0.0f));

    std::cout << "Test8: " << physics::LineIntersectsAABB(line, bBox) << std::endl;

    // Point outside of the cube
    bBox.mWorldMin = glm::vec3(0,0,0);
    bBox.mWorldMax = glm::vec3(1,1,1);

    line.mPosition = glm::vec3(-100.0f,0.5f,0.5f);
    line.mDirection = glm::vec3(0.0f,0.0f,0.0f);

    std::cout << "Test9: " << physics::LineIntersectsAABB(line, bBox) << std::endl;
}