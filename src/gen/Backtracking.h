#pragma once
#include "../Maze.h"
class Backtracking {
    private:
        inline static vector<Maze::CellPosition> cellStack;
    public:
        static void Init(int startingRow, int startingCol, Maze& maze);
        static void Generate(Maze& maze);
};