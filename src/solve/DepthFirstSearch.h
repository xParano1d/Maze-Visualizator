#pragma once
#include "../Maze.h"

class DepthFirstSearch {
    private:
        inline static vector<Maze::CellPosition> cellStack;
        inline static vector<vector<Maze::CellPosition>> parentMap;

        inline static Maze::CellPosition exit;

        inline static bool PathFound;
        inline static Maze::CellPosition cursor;
    public:
        static void Init(int startingRow, int startingCol, int exitRow, int exitCol, Maze& maze);
        static void Solve(Maze &maze);
};