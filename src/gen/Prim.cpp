#include "Prim.h"

void Prim::Init(int startingRow, int startingCol, Grid &maze) {
    currentRow = startingRow;
    currentCol = startingCol;
    
    //Mark as Visited.
    maze.grid[startingRow][startingCol].visited = true;
    maze.grid[startingRow][startingCol].color = {108, 117, 148, 255};

    //Add all valid neighbors of this cell to your Frontier List
    frontierList = maze.GetUnvisitedNeighboursPosition(startingCol, startingRow);
}

void Prim::Generate(Grid &maze) {
    //Pick a Random Unvisited Neighbour
    int randomTarget = GetRandomValue(0, (int)frontierList.size() - 1);
    int TargetRow = frontierList[randomTarget].row;
    int TargetCol = frontierList[randomTarget].col;
    

    //Connect it to the Maze
    vector<Grid::Position> v = maze.VisitedNeighbours(TargetRow, TargetCol);
    int randomNeighbour = GetRandomValue(0, (int)v.size()-1);

    int neighbourRow = TargetRow;
    int neighbourCol = TargetCol;
    Grid::Position neighbourPos;

    switch (v[randomNeighbour]){
        case Grid::Position::LEFT:     //left
            neighbourCol--;
            neighbourPos = Grid::Position::LEFT;
        break;

        case Grid::Position::UP:       //Up
            neighbourRow--;
            neighbourPos = Grid::Position::UP;
        break;

        case Grid::Position::RIGHT:    //Right
            neighbourCol++;
            neighbourPos = Grid::Position::RIGHT;
        break;

        case Grid::Position::DOWN:     //Down
            neighbourRow++;
            neighbourPos = Grid::Position::DOWN;
        break;
    }
    //Remove the Target Cell from the Frontier List
    v.erase(v.begin() + randomNeighbour);
    
    //Remove the wall between the current cell and the chosen cell
    if(!maze.grid[neighbourRow][neighbourCol].visited){
        switch (neighbourPos){
            case Grid::Position::LEFT:
                maze.grid[currentRow][currentCol].leftWall = false;
                maze.grid[neighbourRow][neighbourCol].rightWall = false;
            break;

            case Grid::Position::UP:
                maze.grid[currentRow][currentCol].topWall = false;
                maze.grid[neighbourRow][neighbourCol].bottomWall = false;
            break;

            case Grid::Position::RIGHT:
                maze.grid[currentRow][currentCol].rightWall = false;
                maze.grid[neighbourRow][neighbourCol].leftWall = false;
            break;

            case Grid::Position::DOWN:
                maze.grid[currentRow][currentCol].bottomWall = false;
                maze.grid[neighbourRow][neighbourCol].topWall = false;
            break;
        }

        currentRow = neighbourRow;
        currentCol = neighbourCol;
    }
    //  Mark the target cell as visited
    maze.grid[TargetRow][TargetCol].visited = true;
    maze.grid[TargetRow][TargetCol].color = {108, 117, 148, 255};


    //Expand the Frontier
    vector<Grid::CellPosition> neighboursList = maze.GetUnvisitedNeighboursPosition(TargetRow, TargetCol);

    for(Grid::CellPosition n : neighboursList){
        for(Grid::CellPosition f : frontierList){
            if(f.row != n.row && f.col != n.row){
                frontierList.push_back(n);
            }
        }
    }
}
