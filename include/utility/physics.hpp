#pragma once
// C++ Standard libraries
#include <array>

// Third_party libraries
#include <glm/glm.hpp>

// Own libraries
#include "./components/bounding_box_component.hpp"
#include "./line.hpp"

namespace physics {
    bool Instersects(const BoundingBoxComponent& box1, const BoundingBoxComponent& box2);
    bool IfPointInAABB(const glm::vec3& point, const BoundingBoxComponent& box);
    
    glm::vec3 mtv(const BoundingBoxComponent& box1, const BoundingBoxComponent& box2);
    
    // Works for only one object moving (first box)
    float SweptAABB(const BoundingBoxComponent& box1, const glm::vec3& velocity, const BoundingBoxComponent& box2, glm::vec3& normals);

    // Outputs distance from the starting point of the ray to the bounding box
    // Outputs -1.0f if line never hits the object.
    float LineIntersectsAABB(const Line& line, const BoundingBoxComponent& box);

    bool IsAABBInFrustum(const BoundingBoxComponent& box, const std::array<glm::vec4, 5>& frustumPlanes);

    // Bounding box model creation
    void ExtractInfiniteFrustumPlanes(const glm::mat4& viewProj, std::array<glm::vec4, 5>& planes);
    Model CreateBoundingModel(BoundingBoxComponent& boundingBox);
};