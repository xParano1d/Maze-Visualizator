#pragma once
#include <queue>
#include "../Grid.h"

class BreadthFirstSearch {
    private:
        inline static queue<Grid::CellPosition> frontier;
        inline static vector<vector<Grid::CellPosition>> parentMap;

    public:
        static void Init(int startingRow, int startingCol, Grid &maze);
        static void Generate(Grid& maze);
};