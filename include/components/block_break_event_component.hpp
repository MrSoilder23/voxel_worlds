#pragma once
// Own libraries
#include "./core/component.hpp"

struct BlockBreakEventComponent : public IComponent {
    size_t mEntityID; // Action done by
};