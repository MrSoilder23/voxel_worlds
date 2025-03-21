#include "./utility/physics.hpp"

bool physics::Instersects(const BoundingBoxComponent& box1, const BoundingBoxComponent& box2) {
    return (box1.mWorldMin.x <= box2.mWorldMax.x && box1.mWorldMax.x >= box2.mWorldMin.x) &&
            (box1.mWorldMin.y <= box2.mWorldMax.y && box1.mWorldMax.y >= box2.mWorldMin.y) &&
            (box1.mWorldMin.z <= box2.mWorldMax.z && box1.mWorldMax.z >= box2.mWorldMin.z);
}
bool physics::IfPointInAABB(const glm::vec3& point, const BoundingBoxComponent& box) {
    return (box.mWorldMin.x <= point.x && point.x <= box.mWorldMax.x) &&
            (box.mWorldMin.y <= point.y && point.y <= box.mWorldMax.y) &&
            (box.mWorldMin.z <= point.z && point.z <= box.mWorldMax.z);
}

glm::vec3 physics::mtv(const BoundingBoxComponent& box1, const BoundingBoxComponent& box2) {
    constexpr float epsilon = 0.001f;

    const float overlapX = std::min(box1.mWorldMax.x, box2.mWorldMax.x) - std::max(box1.mWorldMin.x, box2.mWorldMin.x);
    const float overlapY = std::min(box1.mWorldMax.y, box2.mWorldMax.y) - std::max(box1.mWorldMin.y, box2.mWorldMin.y);
    const float overlapZ = std::min(box1.mWorldMax.z, box2.mWorldMax.z) - std::max(box1.mWorldMin.z, box2.mWorldMin.z);

    glm::vec3 mtv(0.0f); // Minimum Translation Vector

    if (overlapX < overlapY && overlapX < overlapZ) {
        float direction = (box1.mWorldMin.x < box2.mWorldMin.x) ? 1.0f : -1.0f;
        mtv = glm::vec3(direction * (overlapX+epsilon), 0.0f, 0.0f);
    } else if (overlapY < overlapZ) {
        float direction = (box1.mWorldMin.y < box2.mWorldMin.y) ? 1.0f : -1.0f;
        mtv = glm::vec3(0.0f, direction * (overlapY+epsilon), 0.0f);
    } else {
        float direction = (box1.mWorldMin.z < box2.mWorldMin.z) ? 1.0f : -1.0f;
        mtv = glm::vec3(0.0f, 0.0f, direction * (overlapZ+epsilon));
    }

    return mtv;
}

// Works for only one object moving (first box)
float physics::SweptAABB(const BoundingBoxComponent& box1, const glm::vec3& velocity, const BoundingBoxComponent& box2, glm::vec3& normals) {
    if ((velocity.x == 0.0f && (box1.mWorldMax.x < box2.mWorldMin.x || box1.mWorldMin.x > box2.mWorldMax.x)) ||
        (velocity.y == 0.0f && (box1.mWorldMax.y < box2.mWorldMin.y || box1.mWorldMin.y > box2.mWorldMax.y)) ||
        (velocity.z == 0.0f && (box1.mWorldMax.z < box2.mWorldMin.z || box1.mWorldMin.z > box2.mWorldMax.z))) {
        normals = glm::vec3(0.0f);
        return 1.0f;
    }

    float xBoxEntry, yBoxEntry, zBoxEntry;
    float xBoxExit, yBoxExit, zBoxExit;

    // Calculate entry and exit distances
    if (velocity.x > 0.0f) {
        xBoxEntry = box2.mWorldMin.x - box1.mWorldMax.x;
        xBoxExit = box2.mWorldMax.x - box1.mWorldMin.x;
    } else {
        xBoxEntry = box2.mWorldMax.x - box1.mWorldMin.x;
        xBoxExit = box2.mWorldMin.x - box1.mWorldMax.x;
    }

    if (velocity.y > 0.0f) {
        yBoxEntry = box2.mWorldMin.y - box1.mWorldMax.y;
        yBoxExit = box2.mWorldMax.y - box1.mWorldMin.y;
    } else {
        yBoxEntry = box2.mWorldMax.y - box1.mWorldMin.y;
        yBoxExit = box2.mWorldMin.y - box1.mWorldMax.y;
    }

    if (velocity.z > 0.0f) {
        zBoxEntry = box2.mWorldMin.z - box1.mWorldMax.z;
        zBoxExit = box2.mWorldMax.z - box1.mWorldMin.z;
    } else {
        zBoxEntry = box2.mWorldMax.z - box1.mWorldMin.z;
        zBoxExit = box2.mWorldMin.z - box1.mWorldMax.z;
    }

    // Calculate entry/exit times
    float xEntry = (velocity.x != 0.0f) ? xBoxEntry / velocity.x : -std::numeric_limits<float>::infinity();
    float xExit = (velocity.x != 0.0f) ? xBoxExit / velocity.x : std::numeric_limits<float>::infinity();

    float yEntry = (velocity.y != 0.0f) ? yBoxEntry / velocity.y : -std::numeric_limits<float>::infinity();
    float yExit = (velocity.y != 0.0f) ? yBoxExit / velocity.y : std::numeric_limits<float>::infinity();

    float zEntry = (velocity.z != 0.0f) ? zBoxEntry / velocity.z : -std::numeric_limits<float>::infinity();
    float zExit = (velocity.z != 0.0f) ? zBoxExit / velocity.z : std::numeric_limits<float>::infinity();

    float entryTime = std::max(std::max(xEntry, yEntry), zEntry);
    float exitTime = std::min(std::min(xExit, yExit), zExit);

    // No collision conditions
    if (entryTime > exitTime || entryTime > 1.0f || exitTime < 0.0f) {
        normals = glm::vec3(0.0f);
        return 1.0f;
    }

    if (entryTime < 0.0f) {
        normals = glm::vec3(0.0f);
        return 0.0f;
    }

    // Determine collision normal
    if (xEntry > yEntry && xEntry > zEntry) { 
        normals.x = (velocity.x > 0.0f) ? -1.0f : 1.0f;
        normals.y = 0.0f; 
        normals.z = 0.0f; 
    } else if (yEntry > zEntry) { 
        normals.y = (velocity.y > 0.0f) ? -1.0f : 1.0f;
        normals.x = 0.0f; 
        normals.z = 0.0f; 
    } else { 
        normals.z = (velocity.z > 0.0f) ? -1.0f : 1.0f;
        normals.x = 0.0f; 
        normals.y = 0.0f; 
    }

    return entryTime;
}

// Outputs distance from the starting point of the ray to the bounding box
// Outputs -1.0f if line never hits the object.
float physics::LineIntersectsAABB(const Line& line, const BoundingBoxComponent& box) {
    float scaleMin = -std::numeric_limits<float>::infinity();
    float scaleMax = std::numeric_limits<float>::infinity();

    if (glm::length(line.mDirection) == 0) {
        return IfPointInAABB(line.mPosition, box) ? 0.0f : -1.0f;
    }

    for(int i = 0; i < 3; i++) {
        float linePos = (&line.mPosition.x)[i];
        float lineDir = (&line.mDirection.x)[i];

        float boxMin = (&box.mWorldMin.x)[i];
        float boxMax = (&box.mWorldMax.x)[i];

        if(lineDir != 0) {
            float scale1 = (boxMin - linePos) / lineDir;
            float scale2 = (boxMax - linePos) / lineDir;
            scaleMin = std::max(scaleMin, std::min(scale1, scale2));
            scaleMax = std::min(scaleMax, std::max(scale1, scale2));
        } else if(linePos < boxMin || linePos > boxMax) {
            return -1.0f;
        }
    }
    
    return (scaleMin <= scaleMax && scaleMax >= 0) ? std::max(scaleMin, 0.0f) : -1.0f;
}

bool physics::IsAABBInFrustum(const BoundingBoxComponent& box, const std::array<glm::vec4, 5>& frustumPlanes) {
    glm::vec3 center = (box.mWorldMin + box.mWorldMax) * 0.5f;
    glm::vec3 halfExtents = (box.mWorldMax - box.mWorldMin) * 0.5f;

    for (int i = 0; i < 5; i++) {
        glm::vec3 normal = glm::vec3(frustumPlanes[i]);
        float distance = frustumPlanes[i].w;

        float radius = glm::dot(halfExtents, glm::abs(normal));
        float centerDistance = glm::dot(normal, center) + distance;

        if (centerDistance + radius < 0) return false;
    }
    return true;
}

// Bounding box model creation
void physics::ExtractInfiniteFrustumPlanes(const glm::mat4& viewProj, std::array<glm::vec4, 5>& planes) {
    glm::mat4 transposed = glm::transpose(viewProj);

    // Left, Right, Bottom, Top, Near (using rows 0,1,2 of the original matrix)
    planes[0] = transposed[3] + transposed[0];
    planes[1] = transposed[3] - transposed[0];
    planes[2] = transposed[3] + transposed[1];
    planes[3] = transposed[3] - transposed[1];
    planes[4] = transposed[3] + transposed[2];
    
    for (auto& plane : planes) {
        float length = glm::length(glm::vec3(plane));
        plane /= length;
    }
}
Model physics::CreateBoundingModel(BoundingBoxComponent& boundingBox) {
    Model model;
    model.vertexPositions = {
        glm::vec3(boundingBox.mLocalMin.x, boundingBox.mLocalMin.y, boundingBox.mLocalMin.z),  // Bottom-left-back
        glm::vec3(boundingBox.mLocalMax.x, boundingBox.mLocalMin.y, boundingBox.mLocalMin.z),  // Bottom-right-back
        glm::vec3(boundingBox.mLocalMax.x, boundingBox.mLocalMax.y, boundingBox.mLocalMin.z),  // Top-right-back
        glm::vec3(boundingBox.mLocalMin.x, boundingBox.mLocalMax.y, boundingBox.mLocalMin.z),  // Top-left-back
        glm::vec3(boundingBox.mLocalMin.x, boundingBox.mLocalMin.y, boundingBox.mLocalMax.z),  // Bottom-left-front
        glm::vec3(boundingBox.mLocalMax.x, boundingBox.mLocalMin.y, boundingBox.mLocalMax.z),  // Bottom-right-front
        glm::vec3(boundingBox.mLocalMax.x, boundingBox.mLocalMax.y, boundingBox.mLocalMax.z),  // Top-right-front
        glm::vec3(boundingBox.mLocalMin.x, boundingBox.mLocalMax.y, boundingBox.mLocalMax.z)   // Top-left-front
    };

    model.indexBufferData = {
        0, 1,  // Bottom edge
        1, 2,  // Right edge
        2, 3,  // Top edge
        3, 0,  // Left edge
    
        // Front face edges
        4, 5,  // Bottom edge
        5, 6,  // Right edge
        6, 7,  // Top edge
        7, 4,  // Left edge
    
        // Connecting edges between back and front faces
        0, 4,  // Bottom-left edge
        1, 5,  // Bottom-right edge
        2, 6,  // Top-right edge
        3, 7   // Top-left edge
    };

    return model;
}