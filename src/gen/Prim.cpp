#include "Prim.h"

void Prim::HighlightFrontier(Maze& maze) {
    for (Maze::CellPosition f : frontierList){
        maze.grid[f.row][f.col].color = {46, 52, 230, 255};
    }
}

void Prim::Init(int startingRow, int startingCol, Maze &maze) {
    //Mark starting Cell as Visited.
    maze.grid[startingRow][startingCol].visited = true;
    maze.grid[startingRow][startingCol].color = {108, 117, 148, 255};

    //Add all valid neighbors of this cell to your Frontier List
    frontierList = maze.GetUnvisitedNeighboursPosition(startingCol, startingRow);
    HighlightFrontier(maze);
}

void Prim::Generate(Maze &maze) {
    if(!frontierList.empty()){  //repeat until

        //Pick a Random Candidate
        int randomTarget = GetRandomValue(0, (int)frontierList.size() - 1);
        int TargetRow = frontierList[randomTarget].row;
        int TargetCol = frontierList[randomTarget].col;
    
    
        //Look at the neighbors of the Target Cell that are Already Visited
        vector<Maze::Direction> v = maze.VisitedNeighbours(TargetRow, TargetCol);
        //Pick one random Visited Neighbour
        int randomNeighbour = GetRandomValue(0, (int)v.size()-1);
    
        //Remove the wall between the current cell and the chosen cell
        switch (v[randomNeighbour]){
            case Maze::Direction::LEFT:
                maze.grid[TargetRow][TargetCol].leftWall = false;
                maze.grid[TargetRow][TargetCol-1].rightWall = false;
            break;
    
            case Maze::Direction::UP:
                maze.grid[TargetRow][TargetCol].topWall = false;
                maze.grid[TargetRow-1][TargetCol].bottomWall = false;
            break;
    
            case Maze::Direction::RIGHT:
                maze.grid[TargetRow][TargetCol].rightWall = false;
                maze.grid[TargetRow][TargetCol+1].leftWall = false;
            break;
    
            case Maze::Direction::DOWN:
                maze.grid[TargetRow][TargetCol].bottomWall = false;
                maze.grid[TargetRow+1][TargetCol].topWall = false;
            break;
        }
    
        //  Mark the Target Cell as visited
        maze.grid[TargetRow][TargetCol].visited = true;
        maze.grid[TargetRow][TargetCol].color = {108, 117, 148, 255};
    
        //Remove the Target Cell from the Frontier List
        frontierList.erase(frontierList.begin() + randomTarget);
    
        //Expand the Frontier:
        vector<Maze::CellPosition> neighboursList = maze.GetUnvisitedNeighboursPosition(TargetRow, TargetCol);
        // Get all neighbors of the NEW cell that are NOT visited yet
        for (Maze::CellPosition n : neighboursList) {
            bool alreadyInList = false;
            for (Maze::CellPosition f : frontierList) {
                if (f.row == n.row && f.col == n.col) {
                    alreadyInList = true;
                    break;
                }
            }
            if (!alreadyInList) {
                frontierList.push_back(n);
            }
        }
    
        HighlightFrontier(maze);
    }else{
        maze.Generated = true;
    }
}
