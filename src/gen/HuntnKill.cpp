#include "HuntnKill.h"

void HuntnKill::Hunt(Grid &maze) {

    vector<Grid::Position> v;
    
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
            case Grid::Position::LEFT:
                neighbourCol--;
                maze.grid[currentRow][currentCol].leftWall = false;
                maze.grid[neighbourRow][neighbourCol].rightWall = false;
            break;

            case Grid::Position::UP:
                neighbourRow--;
                maze.grid[currentRow][currentCol].topWall = false;
                maze.grid[neighbourRow][neighbourCol].bottomWall = false;
            break;

            case Grid::Position::RIGHT:
                neighbourCol++;
                maze.grid[currentRow][currentCol].rightWall = false;
                maze.grid[neighbourRow][neighbourCol].leftWall = false;
            break;

            case Grid::Position::DOWN:
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


void HuntnKill::Init(int startRow, int startCol, Grid &maze) {
    currentRow = startRow;
    currentCol = startCol;

    maze.grid[startRow][startCol].visited = true;
    maze.grid[startRow][startCol].color = {108, 117, 148, 255};

    highlightIterationRow = 0;
    highlightIterationCol = 0;
    pLastCell = nullptr;
}

void HuntnKill::Generate(Grid &maze) {
    if(maze.UnvisitedCount()>0){    //repeat until
    
        Grid::Position neighbourPos;
    
        vector<Grid::Position> v = maze.UnvisitedNeighbours(currentRow, currentCol);
        if(!v.empty()){
            //Perform a random walk:
            int neighbourRow = currentRow;
            int neighbourCol = currentCol;
    
            //  Choose one of the unvisited neighbours
            int random = GetRandomValue(0, (int)v.size()-1);
    
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
    
            //  Remove the wall between the current cell and the chosen cell
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