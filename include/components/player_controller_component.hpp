#pragma once
// Own libraries
#include "./core/component.hpp"

struct PlayerControllerComponent : public IComponent {
    PlayerControllerComponent() : mSpeed(20.0f), mSensitivity(10.0f) {}

    float mSpeed;
    float mSensitivity;
};