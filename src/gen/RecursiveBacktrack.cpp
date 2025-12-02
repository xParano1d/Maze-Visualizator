#include "../Grid.h"
#include <cstdlib>
#include <ctime>

static void Generate(int currentRow, int currentCol, vector<vector<Grid::Cell>> grid){
    //Add the current location to the visited set
    grid[currentRow][currentCol].visited = true;

    //Loop over all four neighbors of the current location in random order
    bool chosenNeighbour = false;
    while(!chosenNeighbour){
        srand(time(0));
        int r = rand() % 5;
        
        switch (r){
        case 0:     //left
            
            break;
        case 1:     //Up
            
            break;
        case 2:     //Right
            
            break;
        case 3:     //Down
            
            break;
        }
    }
    if(currentCol+1 < grid[currentRow].size()){
        if(!grid[currentRow][currentCol+1].visited){
            
        }
    }

}