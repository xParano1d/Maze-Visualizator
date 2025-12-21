#pragma once
#include "../Grid.h"

class WallFollower {
    private:
        inline static int currentRow;
        inline static int currentCol;
        inline static Grid::Position currentDirection;

    public:
        static void Init(int startingRow, int startingCol, Grid& maze);
        static void Solve(Grid &maze);
        inline static bool Solved;
};