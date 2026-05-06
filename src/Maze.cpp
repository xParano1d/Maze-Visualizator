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
            
            if(i!=0 && i!=rows-1){
                if(j!=0 && j!=columns-1){

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
    float percentage = 0.1f;

    int leftoverDeadEndCount = deadEndCount * (1 - percentage);
    int deletedDeadEndCount = deadEndCount - leftoverDeadEndCount;
    cout << stack.size() << ", " << leftoverDeadEndCount << ", " << deletedDeadEndCount<< endl;

    while((int)stack.size() > deletedDeadEndCount){
        //*Choose random Dead End from stack
        int randomDeadEnd = GetRandomValue(0, stack.size()-1);
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
                        grid[currentRow+1][currentCol].topWall = false;
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
            this->grid[i][j] = Cell{true, true, true, true, RED};
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

void Maze::Display(Rectangle DrawingArea, CellPosition startPosition, CellPosition exitPosition) {
    float padding = 20.0f;
    float maxWidth = DrawingArea.width - (padding*2);
    float maxHeight = DrawingArea.height - (padding*2);

    float cellSize = fmin(maxWidth / columns, maxHeight / rows);
    
    float gridWidth = columns * cellSize;
    float gridHeight = rows * cellSize;
    
    float posX = DrawingArea.x + (DrawingArea.width - gridWidth) / 2.0f;
    float posY = DrawingArea.y + (DrawingArea.height - gridHeight) / 2.0f;
    
    float wallThickness = fmax(3.0f, fmin(cellSize * 0.2f, 6.0f));
    float borderThickness = fmax(6.0f, fmin(cellSize * 0.4f, 16.0f));

    //? Background of a Grid
    DrawRectangle(
        (int)(posX - borderThickness), 
        (int)(posY - borderThickness), 
        (int)(gridWidth + borderThickness * 2), 
        (int)(gridHeight + borderThickness * 2), 
        WHITE
    );
    
    //! Drawing Grid
    int startPosX = posX;
    int startPosY = posY;
    
    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->columns; j++){
            //* Background of a Cell
            DrawRectangle(posX, posY, ceil(cellSize), ceil(cellSize), grid[i][j].color);
            
            posX = posX + cellSize;
        }
        posX = startPosX;
        posY = posY + cellSize;
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
            // //* Drawing Walls
            if(this->grid[i][j].rightWall){
                DrawRectangle(posX+cellSize-wallThickness/2, posY-wallThickness/2, wallThickness, cellSize+wallThickness, BLACK);         //! Right Wall
            }
            if(this->grid[i][j].leftWall){
                DrawRectangle(posX-wallThickness/2, posY-wallThickness/2, wallThickness, cellSize + wallThickness, BLACK);               //! Left Wall
            }
            if(this->grid[i][j].topWall){
                DrawRectangle(posX-wallThickness/2, posY-wallThickness/2, cellSize + wallThickness, wallThickness, BLACK);           //! Top Wall
            }
            if(this->grid[i][j].bottomWall){
                DrawRectangle(posX-wallThickness/2, posY + cellSize-wallThickness/2, cellSize + wallThickness, wallThickness, BLACK); //! Bottom Wall
            }
            
            posX = posX + cellSize;
        }
        posX = startPosX;
        posY = posY + cellSize;
    }

    posX = startPosX;
    posY = startPosY;

    float cellCenterX = cellSize / 2;
    float cellCenterY = cellSize / 2;

    float pathThickness = wallThickness*1.4f;

    //Drawing Solution
    if(!deadEndPath.empty()){   //Grey Path (wrong path)
        for(Section sect : deadEndPath){
            float AX = posX + cellCenterX + sect.A.col * cellSize;
            float AY = posY + cellCenterY + sect.A.row * cellSize; 
            
            float BX = posX + cellCenterX + sect.B.col * cellSize;
            float BY = posY + cellCenterY + sect.B.row * cellSize;
            
            DrawLineEx({AX, AY}, {BX, BY}, pathThickness, {108, 117, 148, 255});
        }
    }

    if(!solvePath.empty()){ //* Green Path (Solution)
        //* Drawing whole solution path:
        for(Section sect : solvePath){
    
            float AX = posX + cellCenterX + sect.A.col * cellSize;
            float AY = posY + cellCenterY + sect.A.row * cellSize; 
            
            float BX = posX + cellCenterX + sect.B.col * cellSize;
            float BY = posY + cellCenterY + sect.B.row * cellSize;
            
            DrawLineEx({AX, AY}, {BX, BY}, pathThickness, {27, 227, 84, 255});
        }     
    }
    if(Solved){
        //* two Green lines
        //* 1.  from    the top edge of maze   to   the starting point of path
        DrawLineEx(
            {(float)(posX+cellSize*startPosition.col + cellCenterX), (float)(posY+cellSize*startPosition.row)}, 
            {(float)(posX+cellSize*startPosition.col + cellCenterX), (float)(posY+cellSize*startPosition.row + cellCenterY)}, 
            pathThickness, 
            {27, 227, 84, 255}
        );

        //* 2.  from    ending point of path   to   the bottom edge of maze
        DrawLineEx(
            {(float)(posX+cellSize*exitPosition.col + cellCenterX),(float)(posY+cellSize*exitPosition.row + cellCenterY)}, 
            {(float)(posX+cellSize*exitPosition.col + cellCenterX), (float)(posY+cellSize*exitPosition.row + 2*cellCenterY)}, 
            pathThickness, 
            {27, 227, 84, 255}
        );
    }

    //! Row Highlighting
    if(highlightRowEnabled){
        DrawRectangle(posX, posY+highlightedRow*cellSize, gridWidth, cellSize, highlightColor);
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
