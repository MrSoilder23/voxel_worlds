#pragma once
// C++ standard libraries
#include <vector>
#include <unordered_map>
#include <typeindex>

// Own libraries
#include "component.hpp"

using BlockData = std::unordered_map<std::type_index, std::shared_ptr<IComponent>>;

struct Chunk {
    std::vector<BlockData> blocks; // max size: 32'768 (32*32*32)
};