#include "./utility/circle_loop.hpp"

void CircleLoop::SetCenter(int centerX, int centerZ) {
    mPositionCenterX = centerX;
    mPositionCenterZ = centerZ;
}

std::vector<std::pair<int,int>> CircleLoop::Loop(int radius) {
    std::vector<std::pair<int,int>> coords;

    for(int x = mPositionCenterX - radius; x < mPositionCenterX + radius + 1; x++) {
        for(int z = mPositionCenterZ - radius; z < mPositionCenterZ + radius + 1; z++) {
            if(((x-mPositionCenterX)*(x-mPositionCenterX)) + ((z-mPositionCenterZ)*(z-mPositionCenterZ)) <= (radius*radius)) {
                coords.push_back({x,z});
            }
        }
    }
    
    return coords;
}