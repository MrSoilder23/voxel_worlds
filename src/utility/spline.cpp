#include "./utility/spline.hpp"

Spline::Spline(std::vector<std::pair<float, float>> keypoints) {
    mKeypoints = keypoints;
}

float Spline::evaluate(float x) {
    // Find the segment containing x
    auto it = std::lower_bound(mKeypoints.begin(), mKeypoints.end(), x, 
        [](const auto& a, float b) { return a.first < b; });
    
    if (it == mKeypoints.begin()) return mKeypoints.front().second;
    if (it == mKeypoints.end()) return mKeypoints.back().second;
    
    auto& [x1, y1] = *(it - 1);
    auto& [x2, y2] = *it;
    
    float t = (x - x1) / (x2 - x1);
    return y1 + t * (y2 - y1);
}