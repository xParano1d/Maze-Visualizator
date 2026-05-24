#include "MazeExporter.h"
#include "lib/gif.h"
#include <iostream>
#include <string>
using namespace std;

void MazeExporter::Update(int actionID, float vSpeed, int genSteps, int solveSteps){
    this->vSpeed = vSpeed;

    this->genSteps = genSteps;
    this->solveSteps = solveSteps;

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
    //! ----- this part is only for export with solution ------
    //create 2D boolean array
    vector<vector<bool>> SolutionBools;

    if(includingSolution){
        SolutionBools.resize(maze.rows);

        for (int i = 0; i < maze.rows; i++){
            SolutionBools[i].resize(maze.columns);
            
            for (int j = 0; j < maze.columns; j++){

                SolutionBools[i][j] = false;
                
                Maze::CellPosition cp = {i, j};

                for(Maze::Section sect : maze.solvePath){
                    if(sect.A == cp || sect.B == cp){
                        SolutionBools[i][j] = true;
                    }
                }
            }
        }

    }
    //! -------------------------------------------------------

    string mazeData;  // 2N+1 grid size

    //looping through every maze row:
    for(int i = 0; i < maze.rows; i++){
        
        // celling row (topWall check)
        for(int j = 0; j < maze.columns; j++){
            mazeData += "⬛";    //corner

            if(maze.grid[i][j].topWall){
                mazeData += "⬛";
            }else if(includingSolution && SolutionBools[i][j] && (i == 0 || SolutionBools[i-1][j])){
                mazeData += "🟩";
            }else{
                mazeData += "⬜";
            }
        }
        mazeData += "⬛\n";  //2n _+1_

        // middle row 
        for(int j = 0; j < maze.columns; j++){
            // leftWall check
            if(maze.grid[i][j].leftWall){
                mazeData += "⬛";
            }else if(includingSolution && SolutionBools[i][j] && (j > 0 && SolutionBools[i][j-1])){
                mazeData += "🟩";
            }else{
                mazeData += "⬜";
            }

            // cell center
            if(includingSolution && SolutionBools[i][j]){
                mazeData += "🟩";
            }else{
                mazeData += "⬜";
            }
        }
        mazeData += maze.grid[i][maze.columns - 1].rightWall ? "⬛\n" : "⬜\n";
    }

    // last row:
    for(int i = 0; i < maze.columns; i++){
        // corners
        mazeData += "⬛";

        // bottomWall check
        if(maze.grid[maze.rows-1][i].bottomWall){
            mazeData += "⬛";
        }else if(includingSolution && SolutionBools[maze.rows-1][i]){
            mazeData += "🟩"; 
        }else {
            mazeData += "⬜";
        }
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
    //temporary GPU canvas
    RenderTexture2D canvas = LoadRenderTexture(drawContext.width, drawContext.height);

    //copy original drawContext making it centered on (0,0)
    Rectangle tempContext = {0, 0, drawContext.width, drawContext.height};


    //set drawing to canvas
    BeginTextureMode(canvas);
    ClearBackground(BLANK);

    //force maze display on canvas
    maze.Display(tempContext, {startRow, startCol}, {exitRow, exitCol});
    EndTextureMode();

    //take out of VRAM and save to variable
    Image exportImage = LoadImageFromTexture(canvas.texture);

    // IMAGE PROCESSING:
    ImageFlipVertical(&exportImage);
    ImageAlphaCrop(&exportImage, 0.0f);
    if(!includingSolution){
        ImageColorReplace(&exportImage, {27, 227, 84, 255}, WHITE);
    }
    //exporting
    bool success = ExportImage(exportImage, "maze.png");

    //clean up
    UnloadImage(exportImage);
    UnloadRenderTexture(canvas);

    //export success
    if (success) {
        TraceLog(LOG_INFO, "SUCCESS: PNG Saved!");
    } else {
        TraceLog(LOG_ERROR, "FAILURE: PNG Save failed.");
    }
}


int MazeExporter::CalculateGifDelay(bool alg) {
    if (genSteps <= 0){
        genSteps = 1;
    }

    float targetDurationSeconds = 60.0f * pow(0.5f, vSpeed - 1.0f);


    float savedFrames = (float)genSteps / (float)frameSkip;
    
    if (savedFrames < 1.0f) savedFrames = 1.0f;


    float secondsPerFrame = targetDurationSeconds / savedFrames;
    int delayCentiseconds = (int)(secondsPerFrame * 100.0f);

    if (delayCentiseconds < 2) delayCentiseconds = 2;
    if (delayCentiseconds > 20) delayCentiseconds = 20;
    
    return delayCentiseconds;
}

void MazeExporter::StartGif(int delay, int skipAmount) {
    if (isRecordingGif) return; // Prevent crashing 

    // // Set up the temporary camera
    // gifCamera = LoadRenderTexture(width, height);
    // frameSkip = skipAmount;
    // frameCounter = 0;
    // isRecordingGif = true;

    // // This is the actual function from the gif.h library!
    // GifBegin(&gifRecorder, filename, width, height, delay, 8, false);
}

void MazeExporter::WriteGifFrame(Maze &maze) {
}

void MazeExporter::EndGif() {
}

void MazeExporter::ExportToGif(bool includingSolution) {
    // 1. Calculate the perfect delay based on the UI slider
    int gifDelay = CalculateGifDelay(vSpeed);

    // 2. Start the recorder using your new helper
    StartGif(gifDelay, 5); // Assuming a frameSkip of 5

    // 3. The Time Machine Setup
    SetRandomSeed(maze.Seed);
    
    Maze ghostMaze;
    ghostMaze.CreateEmpty(maze.rows, maze.columns);

    // 4. Speedrun!
    while (!ghostMaze.Generated) {
        // ghostMaze.StepAlgorithm();
        WriteGifFrame(ghostMaze);
    }
    WriteGifFrame(ghostMaze); // Final frame

    // 5. Save the file
    EndGif();
}


void MazeExporter::Import() {
}
