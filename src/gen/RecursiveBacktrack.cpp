#include "../Grid.h"
#include <cstdlib>

void Generate(int startCellX, int startCellY, vector<vector<Grid::Cell>> grid){
    //Add the current location to the visited set
    grid[startCellX][startCellY].visited = true;
    //Loop over all four neighbors of the current location in random order
    

}