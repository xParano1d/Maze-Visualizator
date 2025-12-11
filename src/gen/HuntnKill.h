#pragma once
#include "../Grid.h"

class HuntnKill {
    private:
        inline static int currentRow;
        inline static int currentCol;

        static void Hunt(Grid& maze);
    public:
        static void Init(int startingRow, int startingCol, Grid &maze);
        static void Generate(Grid& maze);
};