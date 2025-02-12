#include "./utility/spiral_loop.hpp"

void SpiralLoop::Loop(int range) {
    // Spiral loop
    if(side) {
        if(mLoopX < max) {
            mLoopX++;
        } else if(mLoopZ < max){
            mLoopZ++;
        }
        if(mLoopZ == max) {
            side = false;
        }
    } else {
        if(mLoopX > -max) {
            mLoopX--;
        } else if(mLoopZ > -max){
            mLoopZ--;
        } 
        if(mLoopZ == -max) {
            side = true;
        }
    }

    if(max <= range) {
        if(mLoopZ == -max) {
            max++;
        } 
    } else if(mLoopX >= max){
        mLoopX = 0;
        mLoopZ = 0;
        max = 1;
        side = true;
    }
}

void SpiralLoop::Reset() {
    mLoopX = 0;
    mLoopZ = 0;
    max = 1;
    side = true;
}

int SpiralLoop::GetLoopX() const {
    return mLoopX;
}
int SpiralLoop::GetLoopZ() const {
    return mLoopZ;
}
