#include "Kruskal.h"
#include <iostream>
using namespace std;

void Kruskal::Init(Grid &maze) {
    //Throw all of the Edges into set
    int h = 0;
    for (int i = 0; i < (int)maze.grid.size(); i++){        
        for (int j = 0; j < (int)maze.grid[i].size(); j++){

            if(j >= 0 && j < (int)maze.grid[i].size()-1){     //Right Edge
                edgeList.push_back({{i, j}, {i, j+1}, Grid::Position::RIGHT});

            }

            if(i >= 0 && i < (int)maze.grid.size()-1){        //Bottom Edge
                edgeList.push_back({{i, j}, {i+1, j},Grid::Position::DOWN});

            }

            maze.grid[i][j].groupID = h;
            h++;
        }
    }
}

void Kruskal::Generate(Grid &maze) {
    if(!Kruskal::edgeList.empty()){ //repeat until

        //Select and Remove an edge from the list at random.
        int random = GetRandomValue(0, (int)edgeList.size()-1);
        Grid::Edge currentEdge = edgeList[random];
    
        edgeList.erase(edgeList.begin()+random);
    
        int rowA = currentEdge.posA.row;
        int colA = currentEdge.posA.col;
    
        int rowB = currentEdge.posB.row;
        int colB = currentEdge.posB.col;
    
        //If the two cells belong to different groups
        if(maze.grid[rowA][colA].groupID != maze.grid[rowB][colB].groupID){
    
            //Merge Cells groups
            maze.ChangeGroupsID(maze.grid[rowB][colB].groupID, maze.grid[rowA][colA].groupID);
    
            //Remove walls between Cells
            switch (currentEdge.direction){
                case Grid::Position::LEFT:
                break;
    
                case Grid::Position::UP:
                break;
    
                case Grid::Position::RIGHT:
                    maze.grid[rowA][colA].rightWall = false;
                    maze.grid[rowB][colB].leftWall = false;
                break;
    
                case Grid::Position::DOWN:
                        maze.grid[rowA][colA].bottomWall = false;
                        maze.grid[rowB][colB].topWall = false;
                break;
            }
    
    
            //Mark the Group as visited
            maze.grid[rowA][colA].visited = true;
            maze.grid[rowA][colA].color = {108, 117, 148, 255};
    
            maze.grid[rowB][colB].visited = true;
            maze.grid[rowB][colB].color = {108, 117, 148, 255};
        }
    
        //Quick fix for Large Edge List full of same group ID's
        bool end = true;
        int sameID = maze.grid[rowA][colA].groupID;
        for (int i = 0; i < (int)maze.grid.size(); i++){        
            for (int j = 0; j < (int)maze.grid[i].size(); j++){
                if(sameID != maze.grid[i][j].groupID){
                    end = false;
                }
            }
        }
        if(end){
            edgeList.clear();
        }
    }else{
        maze.generated = true;
    }
}
