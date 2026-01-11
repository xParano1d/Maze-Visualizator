#include "HuntnKill.h"

void HuntnKill::Hunt(Maze &maze) {

    vector<Maze::Direction> v;
    
    maze.HighlightRow(highlightIterationRow, {27, 227, 84, 255});

    //Scan the grid from the top-left corner to bottom-right
    if(highlightIterationRow >= 0 && highlightIterationRow < (int)maze.grid.size()){        
        if(highlightIterationCol >= 0 && highlightIterationCol < (int)maze.grid[highlightIterationRow].size()){
                    
            if(!unvisitedCellFound){

                //revert color of highlighted cell
                if(pLastCell != nullptr){
                    pLastCell->color = lastCellColor;
                    pLastCell = nullptr;
                }

                //save last cell
                pLastCell = &maze.grid[highlightIterationRow][highlightIterationCol];
                lastCellColor = maze.grid[highlightIterationRow][highlightIterationCol].color;

                //highlight current cell
                maze.grid[highlightIterationRow][highlightIterationCol].color = {27, 227, 84, 255};


                v = maze.VisitedNeighbours(highlightIterationRow, highlightIterationCol);
                if(!maze.grid[highlightIterationRow][highlightIterationCol].visited && v.size() > 0){

                    unvisitedCellFound = true;
                }
            }
            
            if(unvisitedCellFound){
                maze.highlightRowEnabled = false;
                lastCellColor = {108, 117, 148, 255};
                
                currentRow = highlightIterationRow;
                currentCol = highlightIterationCol;
                
            }
            
            highlightIterationCol++;
        }else{
            highlightIterationRow++;
            highlightIterationCol = 0;
        }

    }

    if (unvisitedCellFound){
        int neighbourRow = currentRow;
        int neighbourCol = currentCol;

        switch (v[0]){
            case Maze::Direction::LEFT:
                neighbourCol--;
                maze.grid[currentRow][currentCol].leftWall = false;
                maze.grid[neighbourRow][neighbourCol].rightWall = false;
            break;

            case Maze::Direction::UP:
                neighbourRow--;
                maze.grid[currentRow][currentCol].topWall = false;
                maze.grid[neighbourRow][neighbourCol].bottomWall = false;
            break;

            case Maze::Direction::RIGHT:
                neighbourCol++;
                maze.grid[currentRow][currentCol].rightWall = false;
                maze.grid[neighbourRow][neighbourCol].leftWall = false;
            break;

            case Maze::Direction::DOWN:
                neighbourRow++;
                maze.grid[currentRow][currentCol].bottomWall = false;
                maze.grid[neighbourRow][neighbourCol].topWall = false;
            break;
        }

        maze.grid[currentRow][currentCol].visited = true;
        maze.grid[currentRow][currentCol].color = {108, 117, 148, 255};

        pLastCell = nullptr;        
    }
}


void HuntnKill::Init(int startRow, int startCol, Maze &maze) {
    currentRow = startRow;
    currentCol = startCol;

    maze.grid[startRow][startCol].visited = true;
    maze.grid[startRow][startCol].color = {108, 117, 148, 255};

    highlightIterationRow = 0;
    highlightIterationCol = 0;
    pLastCell = nullptr;
}

void HuntnKill::Generate(Maze &maze) {
    if(maze.UnvisitedCount()>0){    //repeat until
    
        Maze::Direction neighbourPos;
    
        vector<Maze::Direction> v = maze.UnvisitedNeighbours(currentRow, currentCol);
        if(!v.empty()){
            //Perform a random walk:
            int neighbourRow = currentRow;
            int neighbourCol = currentCol;
    
            //  Choose one of the unvisited neighbours
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
    
            //  Remove the wall between the current cell and the chosen cell
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
    
                currentRow = neighbourRow;
                currentCol = neighbourCol;
    
                //  Mark the chosen cell as visited
                maze.grid[neighbourRow][neighbourCol].visited = true;
                maze.grid[neighbourRow][neighbourCol].color = {108, 117, 148, 255};
            }
    
        }else{
            unvisitedCellFound = false;
            Hunt(maze);
        }
    }else{
        maze.generated = true;
    }
}