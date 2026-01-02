#include "BreadthFirstSearch.h"

void BreadthFirstSearch::Init(int startingRow, int startingCol, Grid &maze) {
    maze.UnvisitEveryCell();
    while(!frontier.empty()){
        frontier.pop();
    }
    parentMap.clear();
    parentMap.resize(maze.grid.size(), vector<Grid::CellPosition>(maze.grid[0].size()) );

    frontier.push({startingRow, startingCol});
    maze.grid[startingRow][startingCol].visited = true;


    //Set the Start Cell's parent to "None" (or itself)
    parentMap[0][0] = {0, 0};

    exit = {(int)maze.grid.size()-1, (int)maze.grid[0].size()-1};
}

void BreadthFirstSearch::Solve(Grid &maze) {
    //Repeat this cycle until the Queue is empty or the End is found.
    if(!frontier.empty()){
        //Dequeue
        Grid::CellPosition *pCurrentCell = &frontier.front();
        frontier.pop();

        maze.grid[pCurrentCell->row][pCurrentCell->col].color = {108, 117, 148, 255};

        //Check Victory
        if(pCurrentCell->row == exit.row && pCurrentCell->col == exit.col){
            maze.Solved = true;
        }

        if(!maze.Solved){
            //? Expand "The Wave"
            //Scan and Process Neighbors
            vector<Grid::Position> v = maze.UnvisitedNeighbours(pCurrentCell->row, pCurrentCell->col);

            for(Grid::Position p : v){
                int neighbourRow = pCurrentCell->row;
                int neighbourCol = pCurrentCell->col;

                switch (p){
                    case Grid::Position::LEFT:
                        neighbourCol--;
                    break;
                        
                    case Grid::Position::UP:
                        neighbourRow--;
                    break;

                    case Grid::Position::RIGHT:
                        neighbourCol++;
                    break;
                    
                    case Grid::Position::DOWN:
                        neighbourRow++;
                    break;
                }
                //Mark Visited
                maze.grid[neighbourRow][neighbourCol].visited = true;
                maze.grid[neighbourRow][neighbourCol].color = {46, 52, 230, 255};
                //Set Parent
                parentMap[neighbourRow][neighbourCol] = {pCurrentCell->row, pCurrentCell->col};
                //Enqueue
                frontier.push({neighbourRow, neighbourCol});
            }

            //? Trace the Path

        }
    }
}
