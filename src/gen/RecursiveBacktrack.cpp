#include "RecursiveBacktrack.h"
#include <iostream>

void RecursiveBacktrack::Generate(int currentRow, int currentCol, Grid& maze, bool display){
    //Add the current location to the visited set
    maze.grid[currentRow][currentCol].visited = true;
    maze.grid[currentRow][currentCol].color = {108, 117, 148, 255};

    bool chosenNeighbour = false;
    int neighbourRow;
    int neighbourCol;
    
    Grid::Position neighbourPos;

    int r;
    vector<Grid::Position> v = maze.UnvisitedNeighbours(currentRow, currentCol);
    
    int min = 10;
    for (int n : v){
        if(min >= n){
            min = n;
        }
    }

    int max = -1;
    for (int n : v){
        if(max <= n){
            max = n;
        }
    }
    //Loop over all four neighbors of the current location in random order
    while(!v.empty()){
        r = GetRandomValue(min, max);
        
        switch (r){
            case 0:     //left
                for (int n : v){
                    if(n == Grid::Position::LEFT){
                        chosenNeighbour = true;
                        neighbourRow = currentRow;
                        neighbourCol = currentCol - 1;
                        neighbourPos = Grid::Position::LEFT;
                    }
                }
            break;

            case 1:     //Up
                for (int n : v){
                    if(n == Grid::Position::UP){
                        chosenNeighbour = true;
                        neighbourRow = currentRow - 1;
                        neighbourCol = currentCol;
                        neighbourPos = Grid::Position::UP;
                    }
                }
            break;

            case 2:     //Right
               for (int n : v){
                    if(n == Grid::Position::RIGHT){
                        chosenNeighbour = true;
                        neighbourRow = currentRow;
                        neighbourCol = currentCol + 1;
                        neighbourPos = Grid::Position::RIGHT;
                    }
                }
            break;

            case 3:     //Down
                for (int n : v){
                    if(n == Grid::Position::DOWN){
                        chosenNeighbour = true;
                        neighbourRow = currentRow + 1;
                        neighbourCol = currentCol;
                        neighbourPos = Grid::Position::DOWN;
                    }
                }
            break;

        }
        v.erase(v.begin() + r);
        if(chosenNeighbour){
            std::cout << r << ", " << maze.UnvisitedCount() << "\t" << min << ", " << max << "\t" << v.size() <<std::endl;
            for (int n : v){
                std::cout << n;
            }
            std::cout << std::endl;
            Generate(neighbourRow, neighbourCol, maze, display);
        }   
    }
    
    //Add a passage from the current location to the neighbour
    // if(maze.UnvisitedCount() != 0){
        // //border check
        // switch (neighbourPos){
        //     case Grid::Position::LEFT:
        //         for (int n : v){
        //             if(n != Grid::Position::RIGHT){

        //             }
        //         }
        //         maze.grid[currentRow][currentCol].rightWall = false;
        //         maze.grid[neighbourRow][neighbourCol].leftWall = false;
        //         break;

        //     case Grid::Position::UP:
        //         maze.grid[currentRow][currentCol].topWall = false;
        //         maze.grid[neighbourRow][neighbourCol].bottomWall = false;
        //     break;

        //     case Grid::Position::RIGHT:
        //         maze.grid[currentRow][currentCol].leftWall = false;
        //         maze.grid[neighbourRow][neighbourCol].rightWall = false;
        //     break;

        //     case Grid::Position::DOWN:
        //         maze.grid[currentRow][currentCol].bottomWall = false;
        //         maze.grid[neighbourRow][neighbourCol].topWall = false;
        //     break;
        // }
        
    //     display = true;
        
    // }
}