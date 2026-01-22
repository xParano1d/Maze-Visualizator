#include "Eller.h"
#include <iostream>

void Eller::Init(Maze &maze) {
    maze.ResetGroupsID();

    columnIterator = 0;
    row = 0;
}

void Eller::Generate(Maze &maze) {
    if(!maze.Generated){ 
        if(row < (int)maze.grid.size()-1 && columnIterator < (int)maze.grid[row].size()){ //? horizontal group merging
            //Join adjacent Cells at random
            int idA = maze.grid[row][columnIterator].groupID;
            int idB = maze.grid[row][columnIterator + 1].groupID;
            
            //mark current cell as done
            maze.grid[row][columnIterator].color = {108, 117, 148, 255};
            
            int random = GetRandomValue(1, 100);
            if(random%2==0){
                if(columnIterator+1 < (int)maze.grid[row].size()){
                    if(idA != idB){
                        //Remove walls between Cells conecting them
                        maze.grid[row][columnIterator].rightWall = false;
                        maze.grid[row][columnIterator+1].leftWall = false;
                        
                        //inherit groupID from left cell
                        maze.ChangeGroupsID(idB, idA);
                        
                        //make next cell look the same as current
                        maze.grid[row][columnIterator+1].color = {108, 117, 148, 255};  
                    }
                }
            }
        
            columnIterator++;

            //add every unique id to the stack
            if(columnIterator == (int)maze.grid[row].size()){
                for (Maze::Cell c: maze.grid[row]) {
                    bool pass = true;
                    for(int g : groups){
                        if(g == c.groupID){
                            pass = false;
                        }
                    }
                    if(pass){
                        groups.push_back(c.groupID);
                    }
                }
            }

        }else if(row != (int)maze.grid.size()-1){  //? vertical connections (only if row is not the last maze row)
            if(!groups.empty()){
                int random = GetRandomValue(0, (int)groups.size()-1);
                vector<int> possibleColumns;
            
                for(int i=0; i<(int)maze.grid.size(); i++){
                    if(maze.grid[row][i].groupID == groups[random]){
                        possibleColumns.push_back(i);

                    }
                }
                
                groups.erase(groups.begin() + random);
                
                if(!possibleColumns.empty()){
                    random = GetRandomValue(0, (int)possibleColumns.size() - 1);
                    int randomCol = possibleColumns[random];
                    
                    //a way of making more than 1 vertical connection for bigger groups
                    if((int)possibleColumns.size()>1){
                        random = GetRandomValue(1, 100);
                        if(random % 2 == 0) {
                            groups.push_back(maze.grid[row][randomCol].groupID);
                        }
                    }
                    
                    //Connecting vertically:
                    //connect cells
                    maze.grid[row][randomCol].bottomWall = false;
                    maze.grid[row+1][randomCol].topWall = false;
    
                    maze.grid[row+1][randomCol].color = {108, 117, 148, 255};
    
                    //inherit top cell groupID
                    maze.grid[row + 1][randomCol].groupID = maze.grid[row][randomCol].groupID;
                }
                
            }else{
                columnIterator = 0;
                row++;
            }

        }else{
            //reset columnIterator one time at the start of last row 
            if(columnIterator >= (int)maze.grid[row].size()-1){
                columnIterator = 0;
            }

            if(row == (int)maze.grid.size()-1){ //? Last row connections

                //joining every cell that do not share the same groupID
                if(columnIterator+1 < (int)maze.grid[row].size()){
                    int idA = maze.grid[row][columnIterator].groupID;
                    int idB = maze.grid[row][columnIterator + 1].groupID;
                    //groupID check
                    if(idA != idB){
                        //Remove walls between Cells conecting them
                        maze.grid[row][columnIterator].rightWall = false;
                        maze.grid[row][columnIterator+1].leftWall = false;
                        
                        //make them look the same
                        maze.grid[row][columnIterator].color = {108, 117, 148, 255};
                        maze.grid[row][columnIterator+1].color = {108, 117, 148, 255};

                    }
                    columnIterator++;
                }

                //end check
                if(columnIterator >= (int)maze.grid[row].size()-1){
                    maze.Generated = true;
                }
            
            }
        }
    }

}
