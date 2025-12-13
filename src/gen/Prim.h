#pragma once
#include "../Grid.h"

class Prim {
    private:
        inline static int currentRow;
        inline static int currentCol;

        inline static vector<Grid::CellPosition> frontierList;
    public:
        static void Init(int startingRow, int startingCol, Grid& maze);
        static void Generate(Grid& maze);
};