#include "BreadthFirstSearch.h"

#include <iostream>
using namespace std;

void BreadthFirstSearch::Init(int startingRow, int startingCol, int exitRow, int exitCol, Maze &maze) {
    maze.UnvisitEveryCell();
    while(!frontier.empty()){
        frontier.pop();
    }
    parentMap.clear();
    parentMap.resize(maze.grid.size(), vector<Maze::CellPosition>(maze.grid[0].size()) );

    frontier.push({startingRow, startingCol});
    maze.grid[startingRow][startingCol].visited = true;


    //Set the Start Cell's parent to itself
    parentMap[startingRow][startingCol] = {startingRow, startingCol};

    exit = {exitRow, exitCol};
    PathFound = false;
}

void BreadthFirstSearch::Solve(Maze &maze) {
    //Repeat this cycle until the Queue is empty or the End is found.
    if(!frontier.empty() && !PathFound){
        //Dequeue
        Maze::CellPosition currentCell = frontier.front();
        frontier.pop();

        maze.grid[currentCell.row][currentCell.col].color = {108, 117, 148, 255};
        

        //Check Victory
        if(currentCell == exit){
            PathFound = true;
            cursor = {currentCell.row, currentCell.col};
        }

        if(!maze.Solved){
            //? Expand "The Wave"
            //Scan and Process Neighbors
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
                    //Mark Visited
                    maze.grid[neighbourRow][neighbourCol].visited = true;
                    maze.grid[neighbourRow][neighbourCol].color = {46, 52, 230, 255};
                    //Set Parent
                    parentMap[neighbourRow][neighbourCol] = {currentCell.row, currentCell.col};
                    //Enqueue
                    frontier.push({neighbourRow, neighbourCol});
                }

            }
        }
    }else if(PathFound){
        //? Trace the Path
        Maze::CellPosition parentCell = parentMap[cursor.row][cursor.col];

        if(cursor == parentCell){   //if reached starting cell
            maze.Solved = true; //done
        }else{
            //trace path
            maze.solvePath.push_back({cursor, parentCell});
            //move closer to starting Cell
            cursor = parentCell;
        }

    }
}
