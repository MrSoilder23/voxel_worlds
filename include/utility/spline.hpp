#pragma once
// C++ standard libraries
#include <vector>
#include <algorithm>

class Spline {
    public:
        Spline(const std::vector<std::pair<float, float>>& keypoints);

        float evaluate(float x);
    private:
        struct Segment {
            float x1, x2, y1, y2, dx, dy;
        };

        std::vector<float> mKeypointsX;
        std::vector<float> mKeypointsY;
        std::vector<Segment> mSegments;
        mutable size_t mLastIndex = 0; // Cached segment index

        // Helper for interpolation math
        static float interpolate(const Segment& seg, float x) {
            const float t = (x - seg.x1) / seg.dx;
            return seg.y1 + t * seg.dy;
        }
};