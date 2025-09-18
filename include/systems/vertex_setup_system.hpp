#pragma once
// C++ standard libraries
#include <string>

// Third_party libraries
#include <glad/glad.h>

// Own libraries
#include "bismuth/registry.hpp"
#include "./components/core/mesh.hpp"
#include "./components/bounding_box_component.hpp"

class VertexSetupSystem {
    public:
        void update(bismuth::Registry& registry);

};