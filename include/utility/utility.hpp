#pragma once
// C++ standard libraries
#include <iostream>
#include <memory>
#include <random>
#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>
#include <immintrin.h>

// Third_Party libraries
#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

// Own Libraries
#include "./world/transform.hpp"
#include "./components/bounding_box_component.hpp"
#include "./components/position_component.hpp"
#include "constant.hpp"
#include "./model.hpp"
#include "./line.hpp"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

namespace utility {
    void GetOpenGlVersionInfo();

    // Transform
    void MeshTranslate(Transform& transform, float x, float y, float z);
    void MeshTranslate(Transform& transform, glm::vec3 transformation);
    void MeshRotate(Transform& transform, float angle, glm::vec3 axis);
    void MeshScale(Transform& transform, float x, float y, float z);

    // Position Component tools
    void MovePosition(PositionComponent& positionComponent, const glm::vec3& newPosition);
    void RotatePosition(PositionComponent& positionComponent, const glm::quat& newRotation);
    void ScalePosition(PositionComponent& positionComponent, const glm::vec3& newScale);

    float Smooth(float t);

    char* FastIntToString(char* ptr, int value);
    void FastChunkName(char* ptr, int chunkX, int chunkY, int chunkZ);
}
