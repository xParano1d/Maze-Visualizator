#include "Backtracking.h"

void Backtracking::Init(int startingRow, int startingCol, Maze& maze) {
    cellStack.clear();
    //Choose the initial cell, mark it as visited and push it to the stack
    maze.grid[startingRow][startingCol].visited = true;
    maze.grid[startingRow][startingCol].color = {108, 117, 148, 255};
    cellStack.push_back({startingRow, startingCol});
}

void Backtracking::Generate(Maze &maze) {
    if(!cellStack.empty()){    //repeat until
        ////While the stack is not empty (recursive)
        //one iteration:
        int currentRow = cellStack.back().row;
        int currentCol = cellStack.back().col;
        cellStack.pop_back();
    
    
        Maze::Direction neighbourPos;
    
        //If the current cell has any neighbours which have not been visited
        vector<Maze::Direction> v = maze.UnvisitedNeighbours(currentRow, currentCol);
        if(!v.empty()){
    
            //Push the current cell to the stack
            cellStack.push_back({currentRow, currentCol});
            
            int neighbourRow = currentRow;
            int neighbourCol = currentCol;
    
            //Choose one of the unvisited neighbours
            int random = GetRandomValue(0, (int)v.size()-1);
    
            switch (v[random]){
                case Maze::Direction::LEFT:     //left
                    neighbourCol--;
                    neighbourPos = Maze::Direction::LEFT;
                break;
    
                case Maze::Direction::UP:       //Up
                    neighbourRow--;
                    neighbourPos = Maze::Direction::UP;
                break;
    
                case Maze::Direction::RIGHT:    //Right
                    neighbourCol++;
                    neighbourPos = Maze::Direction::RIGHT;
                break;
    
                case Maze::Direction::DOWN:     //Down
                    neighbourRow++;
                    neighbourPos = Maze::Direction::DOWN;
                break;
            }
            v.erase(v.begin() + random);
    
            //Remove the wall between the current cell and the chosen cell
            if(!maze.grid[neighbourRow][neighbourCol].visited){
                switch (neighbourPos){
                    case Maze::Direction::LEFT:
                        maze.grid[currentRow][currentCol].leftWall = false;
                        maze.grid[neighbourRow][neighbourCol].rightWall = false;
                    break;
    
                    case Maze::Direction::UP:
                        maze.grid[currentRow][currentCol].topWall = false;
                        maze.grid[neighbourRow][neighbourCol].bottomWall = false;
                    break;
    
                    case Maze::Direction::RIGHT:
                        maze.grid[currentRow][currentCol].rightWall = false;
                        maze.grid[neighbourRow][neighbourCol].leftWall = false;
                    break;
    
                    case Maze::Direction::DOWN:
                        maze.grid[currentRow][currentCol].bottomWall = false;
                        maze.grid[neighbourRow][neighbourCol].topWall = false;
                    break;
                }
            }
    
            //Mark the chosen cell as visited and push it to the stack
            maze.grid[neighbourRow][neighbourCol].visited = true;
            maze.grid[neighbourRow][neighbourCol].color = {108, 117, 148, 255};
            cellStack.push_back({neighbourRow, neighbourCol});
            
        }else{
            maze.grid[currentRow][currentCol].color  = {27, 227, 84, 255};
        }
    }else{
        maze.Generated = true;
    }
}
