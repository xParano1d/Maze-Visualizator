#include "Grid.h"
#include <raylib.h>

void Grid::ClearSolution() {
    solvePath.clear();
    deadEndPath.clear();

    Solved = false;
}

void Grid::Create(int rows, int columns) {
    this->rows = rows;
    this->columns = columns;

    this->grid.resize(rows);

    for (int i = 0; i < rows; i++){
        this->grid[i].resize(columns);

        for (int j = 0; j < columns; j++){
            this->grid[i][j] = Cell{true, true, true, true, {255,0,0,255}};
        }
    }

    this->grid[0][0].topWall = false;
    this->grid[rows-1][columns-1].bottomWall = false;

    generated = false;

    Solved = false;
    ClearSolution();
}

void Grid::ChangeEveryCellColor(Color c) {
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            this->grid[i][j].color = c;
        }
    }
}

void Grid::HighlightRow(int row, Color c) {
    highlightRowEnabled = true;

    if(row < 0){
        highlightedRow = 0;
    }else{
        highlightedRow = row;
    }
    
    highlightColor = c;
    highlightColor.a = 90;
}

void Grid::ChangeGroupsID(int fromID, int toID) {
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            if(grid[i][j].groupID == fromID){
                grid[i][j].groupID = toID;
            }
        }
    }
}

void Grid::UnvisitEveryCell() {
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            grid[i][j].visited = false;
        }
    }
}

void Grid::Display() {
    float centerX = GetScreenWidth() / 2;
    float centerY = GetScreenHeight() / 2;
    float aspectRatio = GetScreenWidth() / GetScreenHeight();

    float width = centerX;
    float height = centerX / aspectRatio;

    float offsetX = width / columns;
    float offsetY = height / rows;


    float posX = centerX - width/2; 
    float posY = centerY - height/2;


    float borderThickness = 8;

    //? Background of a Grid
    DrawRectangle(posX-borderThickness, posY-borderThickness/aspectRatio, width+2*borderThickness, height+2*(borderThickness/aspectRatio), WHITE);
    
    //! Drawing Grid
    float startPosX = posX;
    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->columns; j++){
            
            DrawRectangle(posX, posY, offsetX, offsetY, grid[i][j].color);  //* Background of a Cell

            if(this->grid[i][j].rightWall){
                DrawLineEx({posX+offsetX, posY}, {posX+offsetX, posY+offsetY}, 2, BLACK);//! Right Wall
            }
            if(this->grid[i][j].leftWall){
                DrawLineEx({posX, posY}, {posX, posY+offsetY}, 2, BLACK);                //! Left Wall
            }
            if(this->grid[i][j].topWall){
                DrawLineEx({posX, posY}, {posX+offsetX, posY}, 2, BLACK);                //! Top Wall
            }
            if(this->grid[i][j].bottomWall){
                DrawLineEx({posX, posY+offsetY}, {posX+offsetX, posY+offsetY}, 2, BLACK);//! Bottom Wall
            }
            
            posX = posX + offsetX;
        }
        posX = startPosX;
        posY = posY + offsetY;
    }

    posX = centerX - width/2; 
    posY = centerY - height/2;

    float cellCenterX = offsetX / 2;
    float cellCenterY = offsetY / 2;

    //Drawing Solution
    if(!solvePath.empty()){     //* Green Path (Solution)
        for(Section sect : solvePath){
    
            float AX = posX + cellCenterX + sect.A.col * offsetX;
            float AY = posY + cellCenterY + sect.A.row * offsetY; 
            
            float BX = posX + cellCenterX + sect.B.col * offsetX;
            float BY = posY + cellCenterY + sect.B.row * offsetY;
            
            DrawLineEx({AX, AY}, {BX, BY}, 6, {27, 227, 84, 255});
        }
    }
    if(Solved){     //* two Green lines
        //*     from    the top edge of maze   to   the starting point of path
        DrawLineEx({posX+cellCenterX, posY}, {posX+cellCenterX, posY+cellCenterY}, 6, {27, 227, 84, 255});

        //*     from    ending point of path   to   the bottom edge of maze
        DrawLineEx({posX+width-cellCenterX, height}, {posX+width-cellCenterX, posY+height}, 6, {27, 227, 84, 255});
    }

    if(!deadEndPath.empty()){   //Grey Path
        for(Section sect : deadEndPath){

            float AX = posX + cellCenterX + sect.A.col * offsetX;
            float AY = posY + cellCenterY + sect.A.row * offsetY; 
            
            float BX = posX + cellCenterX + sect.B.col * offsetX;
            float BY = posY + cellCenterY + sect.B.row * offsetY;
            
            DrawLineEx({AX, AY}, {BX, BY}, 6, {108, 117, 148, 255});
        }
    }

    //! Row Highlighting
    if(highlightRowEnabled){
        DrawRectangle(posX, posY+highlightedRow*offsetY, width, offsetY, highlightColor);
    }
}

vector<Grid::CellPosition> Grid::GetUnvisitedNeighboursPosition(int cellRow, int cellCol) {
    vector<CellPosition> v;
    if(cellCol > 0 && cellCol < (int)grid[cellRow].size()){         //Left
        if (!grid[cellRow][cellCol - 1].visited) {
            v.push_back({cellRow, cellCol-1});
        }
    }
    if(cellRow > 0 && cellRow < (int)grid.size()){                  //Up
        if(!grid[cellRow-1][cellCol].visited){
            v.push_back({cellRow-1, cellCol});
        }
    }
    if(cellCol+1 > 0  && cellCol+1 < (int)grid[cellRow].size()){    //Right
        if(!grid[cellRow][cellCol+1].visited){
            v.push_back({cellRow, cellCol+1});
        }
    }
    if(cellRow+1 > 0 && cellRow+1 < (int)grid.size()){              //Down
        if(!grid[cellRow+1][cellCol].visited){
            v.push_back({cellRow+1, cellCol});
        }
    }
    return v;
}

vector<Grid::Position> Grid::UnvisitedNeighbours(int cellRow, int cellCol) {
    vector<Position> v;
    if(cellCol > 0 && cellCol < (int)grid[cellRow].size()){         //Left
        if (!grid[cellRow][cellCol - 1].visited) {
            v.push_back(LEFT);
        }
    }
    if(cellRow > 0 && cellRow < (int)grid.size()){                  //Up
        if(!grid[cellRow-1][cellCol].visited){
            v.push_back(UP);
        }
    }
    if(cellCol+1 > 0  && cellCol+1 < (int)grid[cellRow].size()){    //Right
        if(!grid[cellRow][cellCol+1].visited){
            v.push_back(RIGHT);
        }
    }
    if(cellRow+1 > 0 && cellRow+1 < (int)grid.size()){              //Down
        if(!grid[cellRow+1][cellCol].visited){
            v.push_back(DOWN);
        }
    }
    return v;
}

vector<Grid::Position> Grid::VisitedNeighbours(int cellRow, int cellCol) {
    vector<Position> v;
    if(cellCol > 0 && cellCol < (int)grid[cellRow].size()){         //Left
        if (grid[cellRow][cellCol - 1].visited) {
            v.push_back(LEFT);
        }
    }
    if(cellRow > 0 && cellRow < (int)grid.size()){                  //Up
        if(grid[cellRow-1][cellCol].visited){
            v.push_back(UP);
        }
    }
    if(cellCol+1 > 0  && cellCol+1 < (int)grid[cellRow].size()){    //Right
        if(grid[cellRow][cellCol+1].visited){
            v.push_back(RIGHT);
        }
    }
    if(cellRow+1 > 0 && cellRow+1 < (int)grid.size()){              //Down
        if(grid[cellRow+1][cellCol].visited){
            v.push_back(DOWN);
        }
    }
    return v;
}

int Grid::UnvisitedCount() {
    int c=0;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            if(!grid[i][j].visited){
                c++;
            }
        }
    }
    return c;
}
