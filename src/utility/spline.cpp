#include "./utility/spline.hpp"

Spline::Spline(const std::vector<std::pair<float, float>>& keypoints) {
    mKeypointsX.reserve(keypoints.size());
    mKeypointsY.reserve(keypoints.size());
    for (const auto& [x, y] : keypoints) {
        mKeypointsX.push_back(x);
        mKeypointsY.push_back(y);
    }

    // Precompute segment data
    const size_t numSegments = mKeypointsX.size() - 1;
    mSegments.reserve(numSegments);
    for (size_t i = 0; i < numSegments; ++i) {
        const float dx = mKeypointsX[i + 1] - mKeypointsX[i];
        const float dy = mKeypointsY[i + 1] - mKeypointsY[i];
        mSegments.push_back({
            mKeypointsX[i],     // x1
            mKeypointsX[i + 1], // x2
            mKeypointsY[i],     // y1
            mKeypointsY[i + 1], // y2
            dx, dy
        });
    }
}


float Spline::evaluate(float x) {
    if (mLastIndex < mSegments.size()) {
        const auto& cachedSeg = mSegments[mLastIndex];
        if (x >= cachedSeg.x1 && x < cachedSeg.x2) {
            return interpolate(cachedSeg, x);
        }
    }

    // Handle out-of-bounds cases
    if (x <= mSegments.front().x1) {
        mLastIndex = 0;
        return mSegments.front().y1;
    }
    if (x >= mSegments.back().x2) {
        mLastIndex = mSegments.size() - 1;
        return mSegments.back().y2;
    }

    // Choose search strategy based on segment count
    if (mSegments.size() < 64) {
        // Linear search for small datasets
        for (size_t i = 0; i < mSegments.size(); ++i) {
            const auto& seg = mSegments[i];

            if (x >= seg.x1 && x < seg.x2) {
                mLastIndex = i;
                return interpolate(seg, x);
            }
        }
    } else {
        // Binary search for larger datasets
        auto it = std::lower_bound(mKeypointsX.begin(), mKeypointsX.end(), x);
        
        const size_t idx = (it == mKeypointsX.begin()) ? 0 : std::distance(mKeypointsX.begin(), it) - 1;

        mLastIndex = idx;
        return interpolate(mSegments[idx], x);
    }

    return 0.0f;
}