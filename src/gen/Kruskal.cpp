#include "Kruskal.h"

void Kruskal::Init(Maze &maze) {
    //Clearing
    maze.ResetGroupsID();
    edgeList.clear();

    //Throw all of the Edges into set
    int h = 0;
    for (int i = 0; i < (int)maze.grid.size(); i++){        
        for (int j = 0; j < (int)maze.grid[i].size(); j++){

            if(j >= 0 && j < (int)maze.grid[i].size()-1){     //Right Edge
                edgeList.push_back({{i, j}, {i, j+1}, Maze::Direction::RIGHT});
            }

            if(i >= 0 && i < (int)maze.grid.size()-1){        //Bottom Edge
                edgeList.push_back({{i, j}, {i+1, j},Maze::Direction::DOWN});
            }

            maze.grid[i][j].groupID = h;
            h++;
        }
    }
}

void Kruskal::Generate(Maze &maze) {
    
    int rowA, colA, rowB, colB, idA, idB;
    Maze::Edge currentEdge;
    
    bool correctChosen = false;
    while(!correctChosen){  //! looping until we find a good pair to connect
        if(!edgeList.empty()){ //repeat until
            int random = GetRandomValue(0, (int)edgeList.size()-1);

            //Select and Remove an edge from the list at random.
            currentEdge = edgeList[random];
            edgeList.erase(edgeList.begin()+random);
            
            //*A
            rowA = currentEdge.posA.row;
            colA = currentEdge.posA.col;

            idA = maze.grid[rowA][colA].groupID;


            //*B
            rowB = currentEdge.posB.row;
            colB = currentEdge.posB.col;

            idB = maze.grid[rowB][colB].groupID;
            

            //If the two cells belong to different groups
            if(idA != idB){
                correctChosen = true;
            }            
        }else{
            maze.Generated = true;
            correctChosen = true; //stop the loop
        }
    }
    if(correctChosen && !maze.Generated){
        //Merge Cells groups
        maze.ChangeGroupsID(idB, idA);

        //Remove walls between Cells
        switch (currentEdge.direction){
            case Maze::Direction::LEFT:
                correctChosen = false;
            break;

            case Maze::Direction::UP:
                correctChosen = false;
            break;

            case Maze::Direction::RIGHT:
                maze.grid[rowA][colA].rightWall = false;
                maze.grid[rowB][colB].leftWall = false;
            break;

            case Maze::Direction::DOWN:
                maze.grid[rowA][colA].bottomWall = false;
                maze.grid[rowB][colB].topWall = false;
            break;
        }
        if(correctChosen){
            //Connect Group together
            maze.grid[rowA][colA].color = {108, 117, 148, 255};
            maze.grid[rowB][colB].color = {108, 117, 148, 255};
        }
        
    }
    
}
