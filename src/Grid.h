#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma once
#include <vector>
#include <raylib.h>
using namespace std;

class Grid {
    public:
        int rows;
        int columns;
        bool generated;

        struct CellPosition {
            int row;
            int col;

        };
        vector<CellPosition> stack;

        struct Section{
            CellPosition A;
            CellPosition B;
        };
        vector<Section> solvePath;
        vector<Section> deadEndPath;
        bool Solved;
        void ClearSolution();

        enum Position {
            LEFT,
            UP,
            RIGHT,
            DOWN
        };

        struct Edge{
            Grid::CellPosition posA;
            Grid::CellPosition posB;
            Grid::Position direction;   //from A to B 
        };
        
        struct Cell{
            bool rightWall;
            bool leftWall;
            bool topWall;
            bool bottomWall;

            Color color;

            bool visited = false;

            int groupID = 0; 
        };
        vector<vector<Cell>> grid;
        
        void Create(int rows, int columns);
        void Display();
        
        void ChangeEveryCellColor(Color c);
        void HighlightRow(int row, Color c);
        
        void ChangeGroupsID(int fromID, int toID);
        void UnvisitEveryCell();
        
        vector<CellPosition> GetUnvisitedNeighboursPosition(int cellRow, int cellCol);
        vector<Position> UnvisitedNeighbours(int cellRow, int cellCol);
        vector<Position> VisitedNeighbours(int cellRow, int cellCol);
        int UnvisitedCount();


        bool highlightRowEnabled = false;
    private:
        int highlightedRow;
        Color highlightColor = {};
};