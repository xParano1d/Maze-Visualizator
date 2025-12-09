#include "Backtracking.h"

void Backtracking::Generate(int startingRow, int startingCol, Grid &maze) {
    //Choose the initial cell, mark it as visited and push it to the stack
    maze.grid[startingRow][startingCol].visited = true;
    maze.grid[startingRow][startingCol].color = {108, 117, 148, 255};
    maze.stack.push_back({startingRow, startingCol});

    int currentRow;
    int currentCol;
    
    int random;

    int neighbourRow;
    int neighbourCol;
    

    Grid::Position neighbourPos;

    // While the stack is not empty
    while(!maze.stack.empty()){
        currentRow = maze.stack.back().row;
        currentCol = maze.stack.back().col;
        maze.stack.pop_back();

        //If the current cell has any neighbours which have not been visited
        vector<Grid::Position> v = maze.UnvisitedNeighbours(currentRow, currentCol);
        if(!v.empty()){

            //Push the current cell to the stack
            maze.stack.push_back({currentRow, currentCol});
            
            neighbourRow = currentRow;
            neighbourCol = currentCol;

            //Choose one of the unvisited neighbours
            random = GetRandomValue(0, (int)v.size()-1);

            switch (v[random]){
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
            v.erase(v.begin() + random);

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
            }

            //Mark the chosen cell as visited and push it to the stack
            maze.grid[neighbourRow][neighbourCol].visited = true;
            maze.grid[neighbourRow][neighbourCol].color = {108, 117, 148, 255};
            maze.stack.push_back({neighbourRow, neighbourCol});
        }
        maze.Display();
        WaitTime(1);
    }
}
