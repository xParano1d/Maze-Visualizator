#include "BreadthFirstSearch.h"

void BreadthFirstSearch::Init(int startingRow, int startingCol, Grid &maze) {
    maze.UnvisitEveryCell();
    while(!frontier.empty()){
        frontier.pop();
    }
    parentMap.clear();
    parentMap.resize(maze.grid.size(), maze.grid[0].size());

    frontier.push({startingRow, startingCol});
    maze.grid[startingRow][startingCol].visited = true;
    //Set the Start Cell's parent to "None" (or itself)
    parentMap[0][0]
}

void BreadthFirstSearch::Generate(Grid &maze) {

}
