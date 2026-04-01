#include "Maze.h"
#include <iostream>

void Maze::ClearSolution() {
    ChangeEveryCellColor(WHITE);
    
    Solved = false;
    Impossible = false;
    solvePath.clear();
    deadEndPath.clear();
}

void Maze::ClearWalls() {
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            grid[i][j].rightWall = false;
            grid[i][j].leftWall = false;
            grid[i][j].topWall = false;
            grid[i][j].bottomWall = false;

            if(i==0){
                grid[i][j].topWall = true;
            }else if(i==rows-1){
                grid[i][j].bottomWall = true;
            }
            if(j==0){
                grid[i][j].leftWall = true;
            }else if(j==columns-1){
                grid[i][j].rightWall = true;
            }
        }
    }
}

void Maze::ConvertToBraid() {
    vector<CellPosition> stack;

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            
            if(i!=0 || i!=rows-1){
                if(j!=0 || j!=columns-1){

                    //*Dead ends detection:
                    if(grid[i][j].wallCount() >=3){
                        grid[i][j].color = {255, 0, 255, 130};
                        //*Push to stack
                        stack.push_back({i, j});
                    }
                }   
            }

        }
    }
    //*Check how many Dead ends maze have
    int deadEndCount = stack.size();
    float percentage = 0.4f;

    int leftoverDeadEndCount = deadEndCount * (1 - percentage);
    int deletedDeadEndCount = deadEndCount - leftoverDeadEndCount;
    cout << stack.size() << ", " << leftoverDeadEndCount << ", " << deletedDeadEndCount<< endl;

    while(stack.size() > deletedDeadEndCount){
        //*Choose random Dead End from stack
        int randomDeadEnd = GetRandomValue(0, stack.size());
        int randomWall = GetRandomValue(0, 3);
        
        int currentRow = stack[randomDeadEnd].row;
        int currentCol = stack[randomDeadEnd].col;
        
        Cell currentCell = grid[currentRow][currentCol];
        
        //*Check which walls are up
        switch (randomWall){
            case 0: //left
                if(currentCell.leftWall){
                    if(currentCol>0 && currentCol<=columns-1){ //boundary check
                        //* Then remove random wall
                        grid[currentRow][currentCol].leftWall = false;
                        grid[currentRow][currentCol - 1].rightWall = false;
                        //* and whole dead end from stack
                        stack.erase(stack.begin()+randomDeadEnd);
                        grid[currentRow][currentCol].color = {0, 175, 210, 130};
                    }
                }
            break;

            case 1: //right
                if(currentCell.rightWall){
                    if(currentCol>=0 && currentCol<columns-1){
                        grid[currentRow][currentCol].rightWall = false;
                        grid[currentRow][currentCol + 1].leftWall = false;
                        stack.erase(stack.begin()+randomDeadEnd);
                        grid[currentRow][currentCol].color = {0, 175, 210, 130};
                    }
                }
            break;

            case 2: //top 
                if(currentCell.topWall){
                    if(currentRow>0 && currentRow<=rows-1){
                        grid[currentRow][currentCol].topWall = false;
                        grid[currentRow-1][currentCol].bottomWall = false;
                        stack.erase(stack.begin()+randomDeadEnd);
                        grid[currentRow][currentCol].color = {0, 175, 210, 130};
                    }
                }
            break;

            case 3: //bottom
                if(currentCell.bottomWall){
                    if(currentRow>0 && currentRow<rows-1){
                        grid[currentRow][currentCol].bottomWall = false;
                        grid[currentRow-1][currentCol].topWall = false;
                        stack.erase(stack.begin()+randomDeadEnd);
                        grid[currentRow][currentCol].color = {0, 175, 210, 130};
                    }
                }
            break;
            
            default:
                break;
        }
    }
}

void Maze::CreateEmpty(int rows, int columns) {
    this->rows = rows;
    this->columns = columns;

    this->grid.resize(rows);

    for (int i = 0; i < rows; i++){
        this->grid[i].resize(columns);

        for (int j = 0; j < columns; j++){
            this->grid[i][j] = Cell{true, true, true, true, {255,0,0,255}};
        }
    }

    Generated = false;
    Impossible = false;
    
    Solved = false;
    solvePath.clear();
    deadEndPath.clear();

    highlightRowEnabled = false;
    
    prevStartPosition = {-1, -1}; 
    prevExitPosition = {-1, -1};
}

void Maze::ChangeEveryCellColor(Color c) {
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            if(this->grid[i][j].wallCount() < 4){
                this->grid[i][j].color = c;
            }else{
                this->grid[i][j].color = BLACK;
            }
        }
    }
}

void Maze::HighlightRow(int row, Color c) {
    highlightRowEnabled = true;

    if(row < 0){
        highlightedRow = 0;
    }else{
        highlightedRow = row;
    }
    
    highlightColor = c;
    highlightColor.a = 90;
}

void Maze::ResetGroupsID() {
    int x = 1;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            grid[i][j].groupID = x;
            x++;
        }
    }
            
}

void Maze::ChangeGroupsID(int fromID, int toID) {
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            if(grid[i][j].groupID == fromID){
                grid[i][j].groupID = toID;
            }
        }
    }
}

void Maze::UnvisitEveryCell() {
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            grid[i][j].visited = false;
        }
    }
}

void Maze::Display(CellPosition startPosition, CellPosition exitPosition) {
    float centerX = (float)GetScreenWidth() / 2;
    float centerY = (float)GetScreenHeight() / 2;
    float aspectRatio = (float)GetScreenWidth() / (float)GetScreenHeight();

    float width = centerX;
    float height = centerY * aspectRatio;

    float offsetX = width / columns;
    float offsetY = height / rows;

    float cellSize = fmin(offsetX, offsetY);

    float posX = centerX - width/2; 
    float posY = centerY - height/2;


    float wallThickness = fmax(3.0f, fmin(cellSize * 0.2f, 6.0f));
    float borderThickness = wallThickness+3*aspectRatio;

    //? Background of a Grid
    DrawRectangle(posX-borderThickness, posY-borderThickness, width+2*borderThickness, height+2*borderThickness, WHITE);
    
    //! Drawing Grid
    float startPosX = posX;
    float startPosY = posY;

    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->columns; j++){
            //* Background of a Cell
            DrawRectangle(ceil(posX), ceil(posY), ceil(offsetX), ceil(offsetY), grid[i][j].color);
            
            posX = posX + offsetX;
        }
        posX = startPosX;
        posY = posY + offsetY;
    }

    posX = startPosX;
    posY = startPosY;

    //removing wall on top and bottom of maze grid to create entrance and exit
    if (Generated) {
        if (prevStartPosition.row >= 0 && prevStartPosition.row < rows && 
            prevStartPosition.col >= 0 && prevStartPosition.col < columns) {
            grid[prevStartPosition.row][prevStartPosition.col].topWall = true;
        }
        if (prevExitPosition.row >= 0 && prevExitPosition.row < rows && 
            prevExitPosition.col >= 0 && prevExitPosition.col < columns) {
            grid[prevExitPosition.row][prevExitPosition.col].bottomWall = true;
        }

        grid[startPosition.row][startPosition.col].topWall = false;
        grid[exitPosition.row][exitPosition.col].bottomWall = false;

        prevStartPosition = startPosition;
        prevExitPosition = exitPosition;
    }

    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->columns; j++){
            //* Drawing Walls
            if(this->grid[i][j].rightWall){
                DrawRectangle(posX+offsetX-wallThickness/2, posY-wallThickness/2, wallThickness, offsetY+wallThickness, BLACK);         //! Right Wall
            }
            if(this->grid[i][j].leftWall){
                DrawRectangle(posX-wallThickness/2, posY-wallThickness/2, wallThickness, offsetY + wallThickness, BLACK);               //! Left Wall
            }
            if(this->grid[i][j].topWall){
                DrawRectangle(posX-wallThickness/2, posY-wallThickness/2, offsetX + wallThickness, wallThickness, BLACK);           //! Top Wall
            }
            if(this->grid[i][j].bottomWall){
                DrawRectangle(posX-wallThickness/2, posY + offsetY-wallThickness/2, offsetX + wallThickness, wallThickness, BLACK); //! Bottom Wall
            }
            
            posX = posX + offsetX;
        }
        posX = startPosX;
        posY = posY + offsetY;
    }

    posX = startPosX;
    posY = startPosY;

    float cellCenterX = offsetX / 2;
    float cellCenterY = offsetY / 2;

    float pathThickness = wallThickness*1.4f;

    //Drawing Solution
    if(!solvePath.empty()){     //* Green Path (Solution)
        //* two Green lines
        //* 1.  from    the top edge of maze   to   the starting point of path
        DrawLineEx({posX+offsetX*startPosition.col + cellCenterX, posY+offsetY*startPosition.row}, {posX+offsetX*startPosition.col + cellCenterX, posY+offsetY*startPosition.row + cellCenterY}, pathThickness, {27, 227, 84, 255});

        for(Section sect : solvePath){
    
            float AX = posX + cellCenterX + sect.A.col * offsetX;
            float AY = posY + cellCenterY + sect.A.row * offsetY; 
            
            float BX = posX + cellCenterX + sect.B.col * offsetX;
            float BY = posY + cellCenterY + sect.B.row * offsetY;
            
            DrawLineEx({AX, AY}, {BX, BY}, pathThickness, {27, 227, 84, 255});
        }
    }
    if(Solved){
        //* 2.  from    ending point of path   to   the bottom edge of maze
        DrawLineEx({posX+offsetX*exitPosition.col + cellCenterX, posY+offsetY*exitPosition.row + cellCenterY}, {posX+offsetX*exitPosition.col + cellCenterX, posY+offsetY*exitPosition.row + 2*cellCenterY}, pathThickness, {27, 227, 84, 255});
    }

    if(!deadEndPath.empty()){   //Grey Path
        for(Section sect : deadEndPath){
            float AX = posX + cellCenterX + sect.A.col * offsetX;
            float AY = posY + cellCenterY + sect.A.row * offsetY; 
            
            float BX = posX + cellCenterX + sect.B.col * offsetX;
            float BY = posY + cellCenterY + sect.B.row * offsetY;
            
            DrawLineEx({AX, AY}, {BX, BY}, pathThickness, {108, 117, 148, 255});
        }
    }

    //! Row Highlighting
    if(highlightRowEnabled){
        DrawRectangle(posX, posY+highlightedRow*offsetY, width, offsetY, highlightColor);
    }
}

vector<Maze::CellPosition> Maze::GetUnvisitedNeighboursPosition(int cellRow, int cellCol) {
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

vector<Maze::Direction> Maze::Neighbours(CellPosition cell) {
    vector<Direction> v;
    if(cell.col > 0 && cell.col < (int)grid[cell.row].size()){         //Left
        v.push_back(LEFT);
    }
    if(cell.row > 0 && cell.row < (int)grid.size()){                  //Up
        v.push_back(UP);
    }
    if(cell.col+1 > 0  && cell.col+1 < (int)grid[cell.row].size()){    //Right
        v.push_back(RIGHT);
    }
    if(cell.row+1 > 0 && cell.row+1 < (int)grid.size()){              //Down
        v.push_back(DOWN);
    }
    return v;
}

vector<Maze::Direction> Maze::UnvisitedNeighbours(int cellRow, int cellCol) {
    vector<Direction> v;
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

vector<Maze::Direction> Maze::UnvisitedNeighbours(CellPosition cell) {
    vector<Direction> v;
    if(cell.col > 0 && cell.col < (int)grid[cell.row].size()){         //Left
        if (!grid[cell.row][cell.col - 1].visited) {
            v.push_back(LEFT);
        }
    }
    if(cell.row > 0 && cell.row < (int)grid.size()){                  //Up
        if(!grid[cell.row-1][cell.col].visited){
            v.push_back(UP);
        }
    }
    if(cell.col+1 > 0  && cell.col+1 < (int)grid[cell.row].size()){    //Right
        if(!grid[cell.row][cell.col+1].visited){
            v.push_back(RIGHT);
        }
    }
    if(cell.row+1 > 0 && cell.row+1 < (int)grid.size()){              //Down
        if(!grid[cell.row+1][cell.col].visited){
            v.push_back(DOWN);
        }
    }
    return v;
}

vector<Maze::Direction> Maze::VisitedNeighbours(int cellRow, int cellCol) {
    vector<Direction> v;
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

vector<Maze::Direction> Maze::VisitedNeighbours(CellPosition cell) {
    vector<Direction> v;
    if(cell.col > 0 && cell.col < (int)grid[cell.row].size()){         //Left
        if (grid[cell.row][cell.col - 1].visited) {
            v.push_back(LEFT);
        }
    }
    if(cell.row > 0 && cell.row < (int)grid.size()){                  //Up
        if(grid[cell.row-1][cell.col].visited){
            v.push_back(UP);
        }
    }
    if(cell.col+1 > 0  && cell.col+1 < (int)grid[cell.row].size()){    //Right
        if(grid[cell.row][cell.col+1].visited){
            v.push_back(RIGHT);
        }
    }
    if(cell.row+1 > 0 && cell.row+1 < (int)grid.size()){              //Down
        if(grid[cell.row+1][cell.col].visited){
            v.push_back(DOWN);
        }
    }
    return v;
}

int Maze::UnvisitedCount() {
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

int Maze::UnvisitedCount(int row) {
    int c=0;
    for (int j = 0; j < columns; j++){
        if(!grid[row][j].visited){
            c++;
        }
    }
    return c;
}
