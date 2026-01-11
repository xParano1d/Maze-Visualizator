#include "AStar.h"
#include <math.h>

void AStar::Init(int startingRow, int startingCol, int exitRow, int exitCol, Maze &maze) {
    //? Clearing:
    while(!minHeap.empty()){
        minHeap.pop();
    }

    G.clear();
    G.resize(maze.grid.size(), vector<int>(maze.grid[0].size()));

    parentMap.clear();
    parentMap.resize(maze.grid.size(), vector<Maze::CellPosition>(maze.grid[0].size()));

    
    //? Preparing:
    //initialize all to infinity (or laaaaaarge number)
    for (int i = 0; i < (int)G.size(); i++) {
        for (int j = 0; j < (int)G[i].size(); j++) {
            G[i][j] = 10000000;
        }
    }
    //Set Start to zero
    G[startingRow][startingCol] = 0;

    //Set the Start Cell's parent to itself
    parentMap[startingRow][startingCol] = {startingRow, startingCol};

    exit = {exitRow, exitCol};
    PathFound = false;

    //Calculate F for the startNode
    AStarNode startNode;
    startNode.row = startingRow;
    startNode.col = startingCol;
    
    int H = abs(startingRow - exit.row) + abs(startingCol - exit.col);
    startNode.F = H + G[startingRow][startingCol]; //G = 0

    // and push it to the heap
    minHeap.push(startNode);
}

void AStar::Solve(Maze &maze) {
    if(!minHeap.empty() && !PathFound){
        //? Algorithm
        // pop the top node from minHeap
        currentCell = {minHeap.top().row, minHeap.top().col};
        minHeap.pop();

        //mark currentCell as visited (only visually)
        maze.grid[currentCell.row][currentCell.col].color = {108, 117, 148, 255};

        //Check Victory
        if(currentCell == exit){
            PathFound = true;
            cursor = {currentCell.row, currentCell.col};
        }else{
            //process ALL neighbours (that are in bounds of the maze grid)
            vector<Maze::Direction> n = maze.Neighbours(currentCell);

            int tempG = G[currentCell.row][currentCell.col] + 1;
            for(Maze::Direction p : n){
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
                if(validNeighbour && tempG < G[neighbourRow][neighbourCol]){
                    //update cost G
                    G[neighbourRow][neighbourCol] = tempG;
                    //set parent as currentCell
                    parentMap[neighbourRow][neighbourCol] = currentCell;
                    
                    //calculate F Cost
                    float costF = tempG + abs(neighbourRow - exit.row) + abs(neighbourCol - exit.col);
                    //and push neighbour to minHeap
                    minHeap.push({neighbourRow, neighbourCol, costF});

                    maze.grid[neighbourRow][neighbourCol].color = {46, 52, 230, 255};
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
