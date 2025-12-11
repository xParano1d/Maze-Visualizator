#pragma once
#include <vector>
#include <raylib.h>
using std::vector;

class Grid {
    public:
        int rows;
        int columns;

        struct CellPosition {
            int row;
            int col;
        };
        vector<CellPosition> stack;

        enum Position {
            LEFT,
            UP,
            RIGHT,
            DOWN
        };
        vector<Position> UnvisitedNeighbours(int cellRow, int cellCol);
        vector<Position> VisitedNeighbours(int cellRow, int cellCol);
        int UnvisitedCount();

        struct Cell{
            bool rightWall;
            bool leftWall;
            bool topWall;
            bool bottomWall;
            Color color;
            bool visited = false;
        };
        vector<vector<Cell>> grid;

        void Create(int rows, int columns);
        void Display();

        void ChangeEveryCellColor(Color c);
        void HighlightRow(int row, Color c, int time);
    private:
        bool highlightRow = false;
        int highlightedRow;
        Color highlightColor;
        int highlightTime=0;
};