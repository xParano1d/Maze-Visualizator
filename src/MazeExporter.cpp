#include "MazeExporter.h"
#include <iostream>
#include <string>
using namespace std;

void MazeExporter::Update(int actionID) {
    //Exporter handler:
    switch (actionID){
        //!  Export
        //* --- GRID ---
        case 0: //txt
            ExportToTxt(false);
        break;  
        
        case 1: //json
            ExportToJson();
        break;

        case 2: //png
            ExportToPng(false);
        break;

        case 3: //gif
            ExportToGif(false);
        break;
        
        //? --- SOLUTION ---
        case 4: //txt
            ExportToTxt(true);
        break;

        case 5: //png
            ExportToPng(true);
        break;

        case 6: //gif
            ExportToPng(true);
        break;

        //!  Import
        case 7: //from  txt  or  json
            Import();
            break;

    default:
        break;
    }
}

void MazeExporter::ExportToJson() {
    string mazeData;
    //? properties:
    mazeData = "{\n\t\"properties\": {\n";
    mazeData += "\t\t\"rows\": "+ to_string(maze.rows) + ",\n";
    mazeData += "\t\t\"columns\": "+ to_string(maze.columns) + ",\n";
    mazeData += "\t\t\"startRow\": "+ to_string(startRow+1) + ",\n";
    mazeData += "\t\t\"startCol\": "+ to_string(startCol+1) + ",\n";
    mazeData += "\t\t\"exitRow\": "+ to_string(exitRow+1) + ",\n";
    mazeData += "\t\t\"exitCol\": "+ to_string(exitCol+1) + "\n\t},";
    
    //? grid:
    mazeData += "\n\t\"grid\": [\n";
    for(vector<Maze::Cell> row : maze.grid){
        mazeData += "\t\t[";
        for(Maze::Cell cell : row){
            mazeData += " \"";

            mazeData += cell.rightWall ? "1" : "0";
            mazeData += cell.leftWall ? "1" : "0";
            mazeData += cell.topWall ? "1" : "0";
            mazeData += cell.bottomWall ? "1" : "0";

            mazeData += "\", ";
        }

        if (!mazeData.empty()) {
            mazeData.erase(mazeData.length() - 2);
        }
        mazeData += "],\n";
    }

    if (!mazeData.empty()) {
        mazeData.erase(mazeData.length() - 2);
    }
    mazeData += "\n\t]\n}";

    //exporting
    bool success = SaveFileText(".\\maze.json", (char *)mazeData.c_str());

    //export success
    if(success){
        TraceLog(LOG_INFO, "Exported Successfully to {filename}!");
    }else{
        TraceLog(LOG_INFO, "Unable to Export :(");
    }
}

void MazeExporter::ExportToTxt(bool includingSolution) {
    string mazeData;  // 2N+1 grid size
    
    //looping through every maze row:
    for(int i = 0; i < maze.rows; i++){
        // celling row (topWall check)
        for(int j = 0; j < maze.columns; j++){
            mazeData += "⬛";    //corner
            mazeData += maze.grid[i][j].topWall ? "⬛" : "⬜";
        }
        mazeData += "⬛\n";  //2n _+1_

        // middle row 
        for(int j = 0; j < maze.columns; j++){
            mazeData += maze.grid[i][j].leftWall ? "⬛" : "⬜";
            mazeData += "⬜";
        }
        mazeData += maze.grid[i][maze.columns - 1].rightWall ? "⬛\n" : "⬜\n";
    }

    // last row:
    for(int i = 0; i < maze.columns; i++){
        mazeData += "⬛";
        mazeData += maze.grid[maze.rows-1][i].bottomWall ? "⬛" : "⬜";
    }
    mazeData += "⬛";

    //exporting
    bool success = SaveFileText(".\\maze.txt", (char *)mazeData.c_str());

    //export success
    if(success){
        TraceLog(LOG_INFO, "Exported Successfully to {filename}!");
    }else{
        TraceLog(LOG_INFO, "Unable to Export :(");
    }

}

void MazeExporter::ExportToPng(bool includingSolution) {
}

void MazeExporter::ExportToGif(bool includingSolution) {
}

void MazeExporter::Import() {
}
