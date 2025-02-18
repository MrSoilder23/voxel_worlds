#pragma once
// Third_party libraries
#include "./core/component.hpp"

struct GravityComponent : public IComponent {
    GravityComponent() : mMass(1.0f) {}
    
    // Kilograms
    float mMass;
};