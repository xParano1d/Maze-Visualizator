#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma once
#include <vector>
#include <raylib.h>
using namespace std;

class Maze {
    public:
        int rows;
        int columns;
        bool Generated;
        bool Solved;

        struct CellPosition {
            int row;
            int col;

            bool operator==(const CellPosition& other) const{
                return (row == other.row && col == other.col);
            }
            bool operator!=(const CellPosition& other) const{
                return !(*this==other); //using == and negating 
            }

        };

        struct Section{
            CellPosition A;
            CellPosition B;
        };
        vector<Section> solvePath;
        vector<Section> deadEndPath;
        void ClearSolution();

        enum Direction {
            LEFT,
            UP,
            RIGHT,
            DOWN
        };

        struct Edge{
            Maze::CellPosition posA;
            Maze::CellPosition posB;
            Maze::Direction direction;   //from A to B 
        };
        
        struct Cell{
            bool rightWall;
            bool leftWall;
            bool topWall;
            bool bottomWall;

            Color color;

            bool visited = false;


            int groupID = 0;
            
            int wallCount(){
                int count = 0;
                if(rightWall) count++;
                if(leftWall) count++;
                if(topWall) count++;
                if(bottomWall) count++;
                return count;
            };
        };
        vector<vector<Cell>> grid;
        
        void CreateEmpty(int rows, int columns);
        void Display();
        
        void ChangeEveryCellColor(Color c);
        void HighlightRow(int row, Color c);

        void ResetGroupsID();
        void ChangeGroupsID(int fromID, int toID);
        void UnvisitEveryCell();
        
        vector<CellPosition> GetUnvisitedNeighboursPosition(int cellRow, int cellCol);

        vector<Direction> Neighbours(CellPosition cell);

        vector<Direction> UnvisitedNeighbours(int cellRow, int cellCol);
        vector<Direction> UnvisitedNeighbours(CellPosition cell);

        vector<Direction> VisitedNeighbours(int cellRow, int cellCol);
        vector<Direction> VisitedNeighbours(CellPosition cell);

        int UnvisitedCount();
        int UnvisitedCount(int row);

        bool highlightRowEnabled = false;
    private:
        int highlightedRow;
        Color highlightColor = {};
};