#pragma once
// C++ standard libraries
#include <tuple>

struct Tuple3DHash {
    size_t operator()(const std::tuple<int, int, int>&x) const { 
        size_t h1 = std::hash<int>{}(std::get<0>(x));
        size_t h2 = std::hash<int>{}(std::get<1>(x));
        size_t h3 = std::hash<int>{}(std::get<2>(x));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};