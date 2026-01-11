#pragma once
#include "../Maze.h"

class DeadEndFiller {
    private:
    inline static Maze::CellPosition startCell;
    inline static Maze::CellPosition exitCell;
    
    inline static vector<Maze::CellPosition> deadEnd;
    inline static Maze::CellPosition currentCell;
    
    static int getwallCount(int row, int col, Maze &maze);

    inline static bool Filled;
    public:
        static void Init(int startingRow, int startingCol, int endingRow, int endingCol, Maze& maze);
        static void Solve(Maze &maze);
};