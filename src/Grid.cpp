#include "Grid.h"
#include <raylib.h>

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
}

void Grid::ChangeEveryCellColor(Color c) {
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            this->grid[i][j].color = c;
        }
    }
}

void Grid::HighlightRow(int row, Color c, int time) {
    highlightRow = true;

    if(row <= 0){
        highlightedRow = 1;
    }else{
        highlightedRow = row;
    }
    
    highlightColor = c;
    highlightColor.a = 120;

    highlightTime = time;
}

void Grid::Display() {
    float centerX = GetScreenWidth() / 2;
    float centerY = GetScreenHeight() / 2;
    float aspectRatio = GetScreenWidth() / GetScreenHeight();

    float width = centerX;
    float height = centerX / aspectRatio;

    float posX = centerX - width/2;
    float posY = centerY - height/2;


    float offsetX = width / columns;
    float offsetY = height / rows;
    
    float startPosX = posX;


    DrawRectangle(posX-offsetX/3, posY-offsetY/3, width+offsetX/1.5f, height+offsetY/1.5f, WHITE); //? Background of a Grid

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
    
    //Row highlighting
    if(highlightRow){
        if(highlightTime!=0){
            DrawRectangle(startPosX, highlightedRow*offsetY, offsetX*columns, offsetY, highlightColor);
            highlightTime--;
        }else{
            highlightRow = false;
        }
    }
}

vector<Grid::CellPosition> Grid::GetUnvisitedNeighboursPosition(int cellRow, int cellCol) {
    vector<CellPosition> v;
    //Left
    if (!grid[cellRow][cellCol - 1].visited) {
        v.push_back({cellRow, cellCol-1});
    }

    //Up
    if(!grid[cellRow-1][cellCol].visited){
        v.push_back({cellRow-1, cellCol});
    }

    //Right
    if(!grid[cellRow][cellCol+1].visited){
        v.push_back({cellRow, cellCol+1});
    }

    //Down
    if(!grid[cellRow+1][cellCol].visited){
        v.push_back({cellRow+1, cellCol});
    }

    return v;
}

vector<Grid::Position> Grid::UnvisitedNeighbours(int cellRow, int cellCol) {
    vector<Position> v;
    if(cellCol > 0 && cellCol < (int)grid[cellRow].size()){     //Left
        if (!grid[cellRow][cellCol - 1].visited) {
            v.push_back(LEFT);
        }
    }
    if(cellRow > 0 && cellRow < (int)grid.size()){              //Up
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
    if(cellCol > 0 && cellCol < (int)grid[cellRow].size()){     //Left
        if (grid[cellRow][cellCol - 1].visited) {
            v.push_back(LEFT);
        }
    }
    if(cellRow > 0 && cellRow < (int)grid.size()){              //Up
        if(grid[cellRow-1][cellCol].visited){
            v.push_back(UP);
        }
    }
    if(cellCol+1 > 0  && cellCol+1 < (int)grid[cellRow].size()){//Right
        if(grid[cellRow][cellCol+1].visited){
            v.push_back(RIGHT);
        }
    }
    if(cellRow+1 > 0 && cellRow+1 < (int)grid.size()){          //Down
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
