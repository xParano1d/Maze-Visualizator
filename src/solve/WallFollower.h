#pragma once
#include "../Maze.h"

class WallFollower {
    private:
        inline static int previousRow;
        inline static int previousCol;

        inline static int currentRow;
        inline static int currentCol;
        inline static Maze::Direction currentDirection;

    public:
        static void Init(int startingRow, int startingCol, Maze& maze);
        static void Solve(Maze &maze);
};