#include "WallFollower.h"

void WallFollower::Init(int startingRow, int startingCol, Maze &maze) {
    
    currentRow = startingRow;
    currentCol = startingCol;
    
    previousRow = startingRow;
    previousCol = startingCol;
    
    maze.grid[startingRow][startingCol].color = {108, 117, 148, 255};
    currentDirection = Maze::Direction::DOWN;

}

void WallFollower::Solve(Maze &maze) {
    bool right = false;
    bool front = false;

    if(currentRow == maze.rows-1 && currentCol == maze.columns-1){
        maze.Solved = true;
        maze.grid[currentRow][currentCol].color = WHITE;
    }else if(!maze.Solved){
        
        switch (currentDirection){
            case (Maze::Direction::LEFT):
                if(maze.grid[currentRow][currentCol].topWall){  //? is there wall on right?
                    right = true;
                }
                if(maze.grid[currentRow][currentCol].leftWall){ //? is there wall in front?
                    front = true;
                }
            break;
            
            case (Maze::Direction::UP):
                if(maze.grid[currentRow][currentCol].rightWall){
                    right = true;
                }
                if(maze.grid[currentRow][currentCol].topWall){
                    front = true;
                }
            break;
            
            case (Maze::Direction::RIGHT):
                if(maze.grid[currentRow][currentCol].bottomWall){
                    right = true;
                }
                if(maze.grid[currentRow][currentCol].rightWall){
                    front = true;
                }
            break;
            
            case (Maze::Direction::DOWN):
                if(maze.grid[currentRow][currentCol].leftWall){
                    right = true;
                }
                if(maze.grid[currentRow][currentCol].bottomWall){
                    front = true;
                }
                break;

            default:
            break;
        }



        if(right){
            if(front){
                // Turn 90° to the Left. Do not move yet.
                switch (currentDirection){
                    case (Maze::Direction::LEFT):
                    currentDirection = Maze::Direction::DOWN;
                    break;
                
                    case (Maze::Direction::UP):
                        currentDirection = Maze::Direction::LEFT;
                    break;
                
                    case (Maze::Direction::RIGHT):
                        currentDirection = Maze::Direction::UP;
                    break;
                
                    case (Maze::Direction::DOWN):
                        currentDirection = Maze::Direction::RIGHT;
                    break;
                
                    default:
                    break;
                }

            }else{
                //Do not turn. Move forward one step.
                switch (currentDirection){
                    case (Maze::Direction::LEFT):
                        currentCol--;
                    break;
                
                    case (Maze::Direction::UP):
                        currentRow--;
                    break;
                    
                    case (Maze::Direction::RIGHT):
                        currentCol++;
                    break;
                    
                    case (Maze::Direction::DOWN):
                        currentRow++;
                    break;
                    
                    default:
                    break;
                }

            }
        }else{
            //Turn 90° to the Right and move forward one step.
            switch (currentDirection){
                case (Maze::Direction::LEFT):
                    currentDirection = Maze::Direction::UP;
                    currentRow--;
                break;
                    
                case (Maze::Direction::UP):
                    currentDirection = Maze::Direction::RIGHT;
                    currentCol++;
                break;
                    
                case (Maze::Direction::RIGHT):
                    currentDirection = Maze::Direction::DOWN;
                    currentRow++;
                break;
            
                case (Maze::Direction::DOWN):
                    currentDirection = Maze::Direction::LEFT;
                    currentCol--;
                break;
                    
                default:
                break;
                }
                
            }

        
        if(previousRow != currentRow || previousCol != currentCol){
            
            if (!maze.solvePath.empty() && maze.solvePath.back().A.row == currentRow && maze.solvePath.back().A.col == currentCol){

                maze.deadEndPath.push_back(maze.solvePath.back());

                maze.solvePath.pop_back();

            }else{

                maze.solvePath.push_back({{previousRow, previousCol}, {currentRow, currentCol}});
            }

            maze.grid[previousRow][previousCol].color = WHITE;
            maze.grid[currentRow][currentCol].color = {108, 117, 148, 255};

            previousRow = currentRow;
            previousCol = currentCol;
        }
    }
}
