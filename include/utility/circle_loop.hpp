#pragma once
// C++ Standard libraries
#include <vector>

class CircleLoop {
    public:
        void SetCenter(int centerX, int centerZ);

        std::vector<std::pair<int,int>> Loop(int radius);
    private:
        int mPositionCenterX = 0;
        int mPositionCenterZ = 0;
};