#include "HuntnKill.h"

void HuntnKill::Hunt(Grid &maze) {
    bool unvisitedCellFound = false;

    vector<Grid::Position> v;

    //Scan the grid from the top-left corner to bottom-right
    for (int i = 0; i < (int)maze.grid.size(); i++){        
        
        for (int j = 0; j < (int)maze.grid[i].size(); j++){
            if(!unvisitedCellFound){
                v = maze.VisitedNeighbours(i, j);
                if(!maze.grid[i][j].visited && v.size() > 0){
                    currentRow = i;
                    currentCol = j;

                    unvisitedCellFound = true;
                    maze.grid[currentRow][currentCol].color = {27, 227, 84, 255};
                }
            }else{
                break;
            }
        }
        if(unvisitedCellFound){
            maze.HighlightRow(i, {27, 227, 84, 255});
            break;
        }
    }

    if (unvisitedCellFound){
        switch (v[0]){
            case Grid::Position::LEFT:
                currentCol--;
            break;

            case Grid::Position::UP:
                currentRow--;
                break;

            case Grid::Position::RIGHT:
                currentCol++;
            break;

            case Grid::Position::DOWN:
                currentRow++;
            break;
        }
    }

    maze.grid[currentRow][currentCol].visited = true;
}


void HuntnKill::Init(int startRow, int startCol, Grid &maze) {
    currentRow = startRow;
    currentCol = startCol;

    maze.grid[startRow][startCol].visited = true;
    maze.grid[startRow][startCol].color = {108, 117, 148, 255};
}

void HuntnKill::Generate(Grid &maze) {
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
        }
        //  Mark the chosen cell as visited
        maze.grid[neighbourRow][neighbourCol].visited = true;
        maze.grid[neighbourRow][neighbourCol].color = {108, 117, 148, 255};

    }else{
        Hunt(maze);
    }
}