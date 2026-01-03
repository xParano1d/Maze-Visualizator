#pragma once
#include "../Grid.h"

class DeadEndFiller {
    private:
        
    public:
        static void Init(int startingRow, int startingCol, Grid& maze);
        static void Solve(Grid &maze);
};