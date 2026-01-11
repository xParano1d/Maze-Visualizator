#include "DeadEndFiller.h"

int DeadEndFiller::getwallCount(int row, int col, Maze& maze) {
    //check wall count for current cell
    int wallCount = maze.grid[row][col].wallCount();
    //add visited neighbours that are connected as a wallcount
    vector<Maze::Direction> v = maze.VisitedNeighbours(row, col);
    for(Maze::Direction p : v){
        int neighbourRow = row;
        int neighbourCol = col;
        bool validNeighbour = false;
        
        switch (p){
            case Maze::Direction::LEFT:
                neighbourCol--;
                if(!maze.grid[row][col].leftWall && !maze.grid[neighbourRow][neighbourCol].rightWall){
                    validNeighbour = true;
                }
            break;
                
            case Maze::Direction::UP:
                neighbourRow--;
                if(!maze.grid[row][col].topWall && !maze.grid[neighbourRow][neighbourCol].bottomWall){
                    validNeighbour = true;
                }
            break;

            case Maze::Direction::RIGHT:
                neighbourCol++;
                if(!maze.grid[row][col].rightWall && !maze.grid[neighbourRow][neighbourCol].leftWall){
                    validNeighbour = true;
                }
            break;
            
            case Maze::Direction::DOWN:
                neighbourRow++;
                if(!maze.grid[row][col].bottomWall && !maze.grid[neighbourRow][neighbourCol].topWall){
                    validNeighbour = true;
                }
            break;
        }

        if(validNeighbour){
            wallCount++;
        }
    }
    return wallCount;
}

void DeadEndFiller::Init(int startingRow, int startingCol, int endingRow, int endingCol, Maze &maze) {
    deadEnd.clear();
    maze.UnvisitEveryCell();
    
    startCell = {startingRow, startingCol};
    exitCell = {endingRow, endingCol};
    Filled = false;
}
void DeadEndFiller::Solve(Maze &maze) {
    if(!Filled){
        int wallCount;
        if(deadEnd.empty()){
            //search the entire grid
            for (int i = 0; i < (int)maze.grid.size(); i++){
                for (int j = 0; j < (int)maze.grid[0].size(); j++){
                    if (!maze.grid[i][j].visited) { //already blocked?
                        //check wall count for current cell
                        wallCount = getwallCount(i, j, maze);

                        if(wallCount == 3){
                            Maze::CellPosition pos = {i, j};
                            //filling every cell with wallCount>3
                            if( pos!=startCell && pos!=exitCell ){
                                //that isn't a start or an exit
                                maze.grid[i][j].visited = true;
                                maze.grid[i][j].color = {108, 117, 148, 255};
        
                                deadEnd.push_back(pos);
                                break;
                            }
                        }
                    }
                }
                if(!deadEnd.empty()){
                    break;
                }
            }
            if(deadEnd.empty()){
                currentCell = startCell;
                Filled = true;
            }
        }else{
            currentCell = deadEnd.back();
            deadEnd.pop_back();
            
            //and its neighbours (until we get to hallway)
            vector<Maze::Direction> v = maze.UnvisitedNeighbours(currentCell);
            for(Maze::Direction p : v){
                int neighbourRow = currentCell.row;
                int neighbourCol = currentCell.col;
                bool validNeighbour = false;

                switch (p){
                    case Maze::Direction::LEFT:
                        neighbourCol--;
                        if(!maze.grid[currentCell.row][currentCell.col].leftWall && !maze.grid[neighbourRow][neighbourCol].rightWall){
                            validNeighbour = true;
                        }
                    break;
                        
                    case Maze::Direction::UP:
                        neighbourRow--;
                        if(!maze.grid[currentCell.row][currentCell.col].topWall && !maze.grid[neighbourRow][neighbourCol].bottomWall){
                            validNeighbour = true;
                        }
                    break;

                    case Maze::Direction::RIGHT:
                        neighbourCol++;
                        if(!maze.grid[currentCell.row][currentCell.col].rightWall && !maze.grid[neighbourRow][neighbourCol].leftWall){
                            validNeighbour = true;
                        }
                    break;
                    
                    case Maze::Direction::DOWN:
                        neighbourRow++;
                        if(!maze.grid[currentCell.row][currentCell.col].bottomWall && !maze.grid[neighbourRow][neighbourCol].topWall){
                            validNeighbour = true;
                        }
                    break;
                }
                if(validNeighbour){
                    wallCount = getwallCount(neighbourRow, neighbourCol, maze);
                    //still dead end?
                    if(wallCount == 3){
                        // visit
                        maze.grid[neighbourRow][neighbourCol].visited = true;
                        maze.grid[neighbourRow][neighbourCol].color = {108, 117, 148, 255};
                        // and push it back to the stack
                        deadEnd.push_back({neighbourRow, neighbourCol});
                    }
                }
            }
        }
        
        
    }else{
        //Trace path after filling
        if(currentCell!=exitCell){
            Maze::CellPosition neighbourCell = currentCell;
            //where can i go?
            vector<Maze::Direction> v = maze.UnvisitedNeighbours(currentCell);
            while(neighbourCell==currentCell){

                switch (v.back()){
                    case Maze::Direction::LEFT:
                        if(!maze.grid[currentCell.row][currentCell.col].leftWall){
                            neighbourCell.col--;
                        }
                    break;
                        
                    case Maze::Direction::UP:
                        if(!maze.grid[currentCell.row][currentCell.col].topWall){
                            neighbourCell.row--;
                        }
                    break;
    
                    case Maze::Direction::RIGHT:
                        if(!maze.grid[currentCell.row][currentCell.col].rightWall){
                            neighbourCell.col++;
                        }
                    break;
                    
                    case Maze::Direction::DOWN:
                        if(!maze.grid[currentCell.row][currentCell.col].bottomWall){
                            neighbourCell.row++;
                        }
                    break;
                }
                if(neighbourCell==currentCell){
                    v.pop_back();
                }
            }

            //mark as visited
            maze.grid[neighbourCell.row][neighbourCell.col].visited = true;

            //add path to stack
            maze.solvePath.push_back({currentCell, neighbourCell});
            currentCell = neighbourCell;


        }else{
            //path done:
            maze.Solved = true;
        }
    }
}
