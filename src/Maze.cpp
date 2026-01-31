#include "Maze.h"

void Maze::ClearSolution() {
    ChangeEveryCellColor(WHITE);
    
    Solved = false;
    solvePath.clear();
    deadEndPath.clear();
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

    this->grid[0][0].topWall = false;
    this->grid[rows-1][columns-1].bottomWall = false;

    Generated = false;

    Solved = false;
    solvePath.clear();
    deadEndPath.clear();

    highlightRowEnabled = false;
}

void Maze::ChangeEveryCellColor(Color c) {
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            this->grid[i][j].color = c;
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

void Maze::Display() {
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


    float wallThickness = fmax(2.0f, fmin(cellSize * 0.1f, 6.0f));
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

    for (int i = 0; i < this->rows; i++){
        for (int j = 0; j < this->columns; j++){
            //* Drawing Walls
            if(this->grid[i][j].rightWall){
                DrawRectangle(posX+offsetX-wallThickness/2, posY-wallThickness/2, wallThickness, offsetY+wallThickness, BLACK);   //! Right Wall
            }
            if(this->grid[i][j].leftWall){
                DrawRectangle(posX-wallThickness/2, posY-wallThickness/2, wallThickness, offsetY + wallThickness, BLACK);           //! Left Wall
            }
            if(this->grid[i][j].topWall){
                DrawRectangle(posX-wallThickness/2, posY-wallThickness/2, offsetX + wallThickness, wallThickness, BLACK);           //! Top Wall
            }
            if(this->grid[i][j].bottomWall){
                DrawRectangle(posX-wallThickness/2, posY + offsetY-wallThickness/2, offsetX + wallThickness, wallThickness, BLACK);   //! Bottom Wall
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

    float pathThickness = wallThickness*2;

    //Drawing Solution
    if(!solvePath.empty()){     //* Green Path (Solution)
        for(Section sect : solvePath){
    
            float AX = posX + cellCenterX + sect.A.col * offsetX;
            float AY = posY + cellCenterY + sect.A.row * offsetY; 
            
            float BX = posX + cellCenterX + sect.B.col * offsetX;
            float BY = posY + cellCenterY + sect.B.row * offsetY;
            
            DrawLineEx({AX, AY}, {BX, BY}, pathThickness, {27, 227, 84, 255});
        }
    }
    if(Solved){     //* two Green lines
        //*     from    the top edge of maze   to   the starting point of path
        DrawLineEx({posX+cellCenterX, posY}, {posX+cellCenterX, posY+cellCenterY}, pathThickness, {27, 227, 84, 255});

        //*     from    ending point of path   to   the bottom edge of maze
        DrawLineEx({posX+width-cellCenterX, posY+height}, {posX+width-cellCenterX, posY+height-cellCenterY}, pathThickness, {27, 227, 84, 255});
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
