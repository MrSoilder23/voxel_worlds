#pragma once
// C++ standard libraries
#include <vector>
#include <algorithm>

class Spline {
    public:
        Spline(std::vector<std::pair<float, float>> keypoints);

        float evaluate(float x);
    private:
        std::vector<std::pair<float, float>> mKeypoints;
};