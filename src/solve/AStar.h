#pragma once
#include "../Maze.h"
#include <queue>

class AStar {
    private:
        struct AStarNode {
            int row;
            int col;
            float F;
        };
        struct NodeCompare {// Min-heap logic: 
            bool operator()(const AStarNode& A, const AStarNode& B) {
                return A.F > B.F; //Smallest F Cost gets priority
            }
        };
        inline static priority_queue<AStarNode, vector<AStarNode>, NodeCompare> minHeap;
        
        inline static vector<vector<int>> G;


        inline static Maze::CellPosition currentCell;
        inline static Maze::CellPosition exit;


        inline static bool PathFound;
        inline static vector<vector<Maze::CellPosition>> parentMap;
        inline static Maze::CellPosition cursor;

    public:
        static void Init(int startingRow, int startingCol, int exitRow, int exitCol, Maze &maze);
        static void Solve(Maze &maze);
};