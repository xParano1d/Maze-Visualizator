#pragma once
#include "../Maze.h"

class Prim {
    private:
        inline static void HighlightFrontier(Maze& maze);
    public:
        inline static vector<Maze::CellPosition> frontierList;

        static void Init(int startingRow, int startingCol, Maze& maze);
        static void Generate(Maze& maze);
};