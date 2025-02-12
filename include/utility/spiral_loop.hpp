#pragma once

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