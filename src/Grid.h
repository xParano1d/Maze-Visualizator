#pragma once
#include <vector>
#include <raylib.h>
using std::vector;

class Grid {
    public:
        int rows;
        int columns;

        struct Cell{
            bool rightWall;
            bool leftWall;
            bool topWall;
            bool bottomWall;
            Color color;
            bool visited = false;
        };
    
        void Create(int rows, int columns);
        void Display();
        vector<vector<Cell>> grid;

        enum Position {
            LEFT,
            UP,
            RIGHT,
            DOWN
        };
        vector<Position> UnvisitedNeighbours(int cellRow, int cellCol);
        int UnvisitedCount();
};