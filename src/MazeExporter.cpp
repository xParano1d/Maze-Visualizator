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
    string mazeData = "{\n\t\"properties\":{\n";
    //properties:
    mazeData += "\t\t\"rows\": "+ to_string(maze.rows) + ",\n";
    mazeData += "\t\t\"columns\": "+ to_string(maze.columns) + ",\n";
    mazeData += "\t\t\"startRow\": "+ to_string(startRow+1) + ",\n";
    mazeData += "\t\t\"startCol\": "+ to_string(startCol+1) + ",\n";
    mazeData += "\t\t\"exitRow\": "+ to_string(exitRow+1) + ",\n";
    mazeData += "\t\t\"exitCol\": "+ to_string(exitCol+1) + ",\n\t}\n}";

    //exporting
    bool success = SaveFileText(".\\t.json", (char*)mazeData.c_str());

    //export success
    if(success){
        TraceLog(LOG_INFO, "Exported Successfully!!");
    }else{
        TraceLog(LOG_INFO, "Unable to Export! :(");
    }
}

void MazeExporter::ExportToTxt(bool includingSolution) {
    
}

void MazeExporter::ExportToPng(bool includingSolution) {
}

void MazeExporter::ExportToGif(bool includingSolution) {
}

void MazeExporter::Import() {
}
