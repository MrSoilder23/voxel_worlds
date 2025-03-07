#pragma once
// C++ standard libraries
#include <vector>

// Own libraries
#include "./core/component.hpp"
#include "./components/bounding_box_component.hpp"
#include "./group.hpp"

struct BoundingBoxCollectionComponent : public IComponent{
    std::vector<BoundingBoxComponent> boundingBoxes;

    Group group;
    Group mask;
};