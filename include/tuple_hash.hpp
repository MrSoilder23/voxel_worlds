#pragma once
// C++ standard libraries
#include <tuple>

struct Tuple3DHash {
    size_t operator()(const std::tuple<int, int, int>&x) const
  { 
    return get<0>(x) ^ get<1>(x) ^ get<2>(x); 
  } 
};