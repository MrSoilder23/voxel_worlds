#include "position_component.hpp"

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