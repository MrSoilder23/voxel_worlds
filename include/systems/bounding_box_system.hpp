#pragma once
// Own libraries
#include "components/bounding_box_component.hpp"
#include "components/position_component.hpp"
#include "utility/physics.hpp"
#include "utility/constant.hpp"
#include "bismuth/registry.hpp"

#include <tbb/tbb.h>

class BoundingBoxSystem {
    public:
        void update(bismuth::Registry& entityManager);
};