#pragma once
#include "../Grid.h"

class RecursiveBacktrack {
    public:
        static void Generate(int currentRow, int currentCol, Grid& maze);
};