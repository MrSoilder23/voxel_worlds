#include "./utility/physics.hpp"

bool physics::Instersects(const BoundingBoxComponent& box1, const BoundingBoxComponent& box2) {
    return (box1.worldMin.x <= box2.worldMax.x && box1.worldMax.x >= box2.worldMin.x) &&
            (box1.worldMin.y <= box2.worldMax.y && box1.worldMax.y >= box2.worldMin.y) &&
            (box1.worldMin.z <= box2.worldMax.z && box1.worldMax.z >= box2.worldMin.z);
}
bool physics::IfPointInAABB(const glm::vec3& point, const BoundingBoxComponent& box) {
    return (box.worldMin.x <= point.x && point.x <= box.worldMax.x) &&
            (box.worldMin.y <= point.y && point.y <= box.worldMax.y) &&
            (box.worldMin.z <= point.z && point.z <= box.worldMax.z);
}

glm::vec3 physics::mtv(const BoundingBoxComponent& box1, const BoundingBoxComponent& box2) {
    constexpr float epsilon = 0.001f;

    const float overlapX = std::min(box1.worldMax.x, box2.worldMax.x) - std::max(box1.worldMin.x, box2.worldMin.x);
    const float overlapY = std::min(box1.worldMax.y, box2.worldMax.y) - std::max(box1.worldMin.y, box2.worldMin.y);
    const float overlapZ = std::min(box1.worldMax.z, box2.worldMax.z) - std::max(box1.worldMin.z, box2.worldMin.z);

    glm::vec3 mtv(0.0f); // Minimum Translation Vector

    if (overlapX < overlapY && overlapX < overlapZ) {
        float direction = (box1.worldMin.x < box2.worldMin.x) ? 1.0f : -1.0f;
        mtv = glm::vec3(direction * (overlapX+epsilon), 0.0f, 0.0f);
    } else if (overlapY < overlapZ) {
        float direction = (box1.worldMin.y < box2.worldMin.y) ? 1.0f : -1.0f;
        mtv = glm::vec3(0.0f, direction * (overlapY+epsilon), 0.0f);
    } else {
        float direction = (box1.worldMin.z < box2.worldMin.z) ? 1.0f : -1.0f;
        mtv = glm::vec3(0.0f, 0.0f, direction * (overlapZ+epsilon));
    }

    return mtv;
}

// Works for only one object moving (first box)
float physics::SweptAABB(const BoundingBoxComponent& box1, const glm::vec3& velocity, const BoundingBoxComponent& box2, glm::vec3& normals) {
    if ((velocity.x == 0.0f && (box1.worldMax.x < box2.worldMin.x || box1.worldMin.x > box2.worldMax.x)) ||
        (velocity.y == 0.0f && (box1.worldMax.y < box2.worldMin.y || box1.worldMin.y > box2.worldMax.y)) ||
        (velocity.z == 0.0f && (box1.worldMax.z < box2.worldMin.z || box1.worldMin.z > box2.worldMax.z))) {
        normals = glm::vec3(0.0f);
        return 1.0f;
    }

    float xBoxEntry, yBoxEntry, zBoxEntry;
    float xBoxExit, yBoxExit, zBoxExit;

    // Calculate entry and exit distances
    if (velocity.x > 0.0f) {
        xBoxEntry = box2.worldMin.x - box1.worldMax.x;
        xBoxExit = box2.worldMax.x - box1.worldMin.x;
    } else {
        xBoxEntry = box2.worldMax.x - box1.worldMin.x;
        xBoxExit = box2.worldMin.x - box1.worldMax.x;
    }

    if (velocity.y > 0.0f) {
        yBoxEntry = box2.worldMin.y - box1.worldMax.y;
        yBoxExit = box2.worldMax.y - box1.worldMin.y;
    } else {
        yBoxEntry = box2.worldMax.y - box1.worldMin.y;
        yBoxExit = box2.worldMin.y - box1.worldMax.y;
    }

    if (velocity.z > 0.0f) {
        zBoxEntry = box2.worldMin.z - box1.worldMax.z;
        zBoxExit = box2.worldMax.z - box1.worldMin.z;
    } else {
        zBoxEntry = box2.worldMax.z - box1.worldMin.z;
        zBoxExit = box2.worldMin.z - box1.worldMax.z;
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

        float boxMin = (&box.worldMin.x)[i];
        float boxMax = (&box.worldMax.x)[i];

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
    glm::vec3 center = (box.worldMin + box.worldMax) * 0.5f;
    glm::vec3 halfExtents = (box.worldMax - box.worldMin) * 0.5f;

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