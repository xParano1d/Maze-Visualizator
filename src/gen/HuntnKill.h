#pragma once
#include "../Maze.h"

class HuntnKill {
    private:
        inline static int highlightIterationRow;
        inline static int highlightIterationCol;
        inline static Maze::Cell *pLastCell;
        inline static Color lastCellColor;
        inline static bool unvisitedCellFound;

        inline static int currentRow;
        inline static int currentCol;

        static void Hunt(Maze& maze);
    public:
        static void Init(int startingRow, int startingCol, Maze &maze);
        static void Generate(Maze& maze);
};