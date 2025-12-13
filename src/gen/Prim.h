#pragma once
#include "../Grid.h"

class Prim {
    private:
        inline static void HighlightFrontier(Grid& maze);
    public:
        inline static vector<Grid::CellPosition> frontierList;

        static void Init(int startingRow, int startingCol, Grid& maze);
        static void Generate(Grid& maze);
};