#include "WallFollower.h"

void WallFollower::Init(int startingRow, int startingCol, Grid &maze) {
    maze.UnvisitEveryCell();

    currentRow = startingRow;
    currentCol = startingCol;
    maze.grid[currentRow][currentCol].visited = true;

    currentDirection = Grid::Position::DOWN;

    Solved = false;
}

void WallFollower::Solve(Grid &maze) {
    bool right = false;
    bool front = false;
    bool left = false;

    //? is there wall on right?
    switch (currentDirection){
        case (Grid::Position::LEFT):
            if(maze.grid[currentRow][currentCol].topWall){
                right = true;
            }
        break;
        
        case (Grid::Position::UP):
            if(maze.grid[currentRow][currentCol].rightWall){
                right = true;
            }
        break;
        
        case (Grid::Position::RIGHT):
            if(maze.grid[currentRow][currentCol].bottomWall){
                right = true;
            }
        break;
        
        case (Grid::Position::DOWN):
            if(maze.grid[currentRow][currentCol].leftWall){
                right = true;
            }
        break;

        default:
        break;
    }

    //? is there wall in front?
    switch (currentDirection){
        case (Grid::Position::LEFT):
            if(maze.grid[currentRow][currentCol].leftWall){
                front = true;
            }
        break;
        
        case (Grid::Position::UP):
            if(maze.grid[currentRow][currentCol].topWall){
                front = true;
            }
        break;
        
        case (Grid::Position::RIGHT):
            if(maze.grid[currentRow][currentCol].rightWall){
                front = true;
            }
        break;
        
        case (Grid::Position::DOWN):
            if(maze.grid[currentRow][currentCol].bottomWall){
                front = true;
            }
        break;

        default:
        break;
    }

    // //? is there wall on left?
    // switch (currentDirection){
    //     case (Grid::Position::LEFT):
    //         if(maze.grid[currentRow][currentCol].bottomWall){
    //             left = true;
    //         }
    //     break;
        
    //     case (Grid::Position::UP):
    //         if(maze.grid[currentRow][currentCol].leftWall){
    //             left = true;
    //         }
    //     break;
        
    //     case (Grid::Position::RIGHT):
    //         if(maze.grid[currentRow][currentCol].topWall){
    //             left = true;
    //         }
    //     break;
        
    //     case (Grid::Position::DOWN):
    //         if(maze.grid[currentRow][currentCol].rightWall){
    //             left = true;
    //         }
    //     break;

    //     default:
    //     break;
    // }


    if(right){
        //You cannot go right. Proceed to step 2.
        if(front){
            //You cannot go straight. Proceed to step 3.
            // if(left){
                // Turn 90° to the Left. Do not move yet.
            // }
            
        }else{
            //Do not turn. Move forward one step.

        }
    }else{
        //Turn 90° to the Right and move forward one step.
    }
}
