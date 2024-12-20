#include "position_component.hpp"

PositionComponent::PositionComponent() : x(0), y(0), z(0) {
}
PositionComponent::PositionComponent(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
}

void PositionComponent::Initialize() {
}

void PositionComponent::SetPosition(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
}

glm::vec3 PositionComponent::GetPosition() {
    return glm::vec3(x,y,z);
}
float& PositionComponent::GetPositionX() {
    return x;
}
float& PositionComponent::GetPositionY() {
    return y;
}
float& PositionComponent::GetPositionZ() {
    return z;
}