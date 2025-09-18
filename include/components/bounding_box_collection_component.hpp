#pragma once
// C++ standard libraries
#include <vector>

// Own libraries
#include "components/bounding_box_component.hpp"
#include "group.hpp"

struct BoundingBoxCollectionComponent {
    std::vector<BoundingBoxComponent> boundingBoxes;

    Group group;
    Group mask;
};