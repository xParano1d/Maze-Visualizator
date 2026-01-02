#pragma once
#include <queue>
#include "../Grid.h"

class BreadthFirstSearch {
    private:
        inline static queue<Grid::CellPosition> frontier;
        inline static vector<vector<Grid::CellPosition>> parentMap;
        inline static Grid::CellPosition exit;

    public:
        static void Init(int startingRow, int startingCol, Grid &maze);
        static void Solve(Grid& maze);
};