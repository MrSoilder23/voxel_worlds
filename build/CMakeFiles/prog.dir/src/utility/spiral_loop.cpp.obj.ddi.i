# 0 "C:/Projects/voxel_worlds/src/utility/spiral_loop.cpp"
# 1 "C:\\Projects\\voxel_worlds\\build//"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "C:/Projects/voxel_worlds/src/utility/spiral_loop.cpp"
# 1 "C:/Projects/voxel_worlds/include/utility/spiral_loop.hpp" 1
       

class SpiralLoop {
    public:
        void Loop(int range);
        void Reset();

        int GetLoopX() const;
        int GetLoopZ() const;

    private:
        int mLoopX = 0;
        int mLoopZ = 0;
        int max = 1;
        bool side = true;
};
# 2 "C:/Projects/voxel_worlds/src/utility/spiral_loop.cpp" 2

void SpiralLoop::Loop(int range) {

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
