#pragma once
#include <queue>
#include "../Maze.h"

class BreadthFirstSearch {
    private:
        inline static queue<Maze::CellPosition> frontier;
        inline static vector<vector<Maze::CellPosition>> parentMap;
        inline static Maze::CellPosition exit;

        inline static bool PathFound;
        inline static Maze::CellPosition cursor;

    public:
        static void Init(int startingRow, int startingCol,  int exitRow, int exitCol, Maze &maze);
        static void Solve(Maze& maze);
};