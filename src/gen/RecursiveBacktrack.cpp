#include "RecursiveBacktrack.h"
#include <iostream>

void RecursiveBacktrack::Generate(int currentRow, int currentCol, Grid& maze){
    //Add the current location to the visited set
    maze.grid[currentRow][currentCol].visited = true;
    maze.grid[currentRow][currentCol].color = {108, 117, 148, 255};

    int neighbourRow;
    int neighbourCol;
    
    Grid::Position neighbourPos;

    int r;
    vector<Grid::Position> v = maze.UnvisitedNeighbours(currentRow, currentCol);

    //Loop over all four neighbors of the current location in random order
    while(!v.empty()){
        r = GetRandomValue(0, (int)v.size()-1);

        neighbourRow = currentRow;
        neighbourCol = currentCol;

        switch (v[r]){
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
        v.erase(v.begin() + r);
        
        if(!maze.grid[neighbourRow][neighbourCol].visited){
            //Add a passage from the current location to the neighbour
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
            Generate(neighbourRow, neighbourCol, maze);
        }   
    }
        
}