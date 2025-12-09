#pragma once
#include "../Grid.h"
class Backtracking {
    public:
        static void Init(int startingRow, int startingCol, Grid& maze);
        static void Generate(Grid& maze);
};