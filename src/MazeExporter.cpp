#include "MazeExporter.h"
#include "../lib/gif.h"
#include "../lib/json.hpp"
#include "../lib/tinyfiledialogs.h"
#include <fstream>
using namespace std;
using json = nlohmann::json;

MazeExporter::MazeExporter(Maze &MazeToExport, int &startRow, int &startCol, int &exitRow, int &exitCol, Rectangle drawContext) : maze(MazeToExport), startRow(startRow), startCol(startCol), exitRow(exitRow), exitCol(exitCol), drawContext(drawContext) {
    gifRecorder = (void*)(new GifWriter);
}

MazeExporter::~MazeExporter() {
    delete (GifWriter*)gifRecorder;
}

void MazeExporter::Update(int actionID, Gui::Algorithm choosenAlgorithm, float vSpeed, int genSteps, int solveSteps){
    switch (choosenAlgorithm){
        case (Gui::Algorithm::Backtracking):    genAlgorithm = choosenAlgorithm; break;
        case (Gui::Algorithm::HuntNKill):   genAlgorithm = choosenAlgorithm; break;
        case (Gui::Algorithm::Prim):    genAlgorithm = choosenAlgorithm; break;
        case (Gui::Algorithm::Kruskal):   genAlgorithm = choosenAlgorithm; break;
        case (Gui::Algorithm::Eller):   genAlgorithm = choosenAlgorithm; break;
        case (Gui::Algorithm::Sidewinder):  genAlgorithm = choosenAlgorithm; break;
        case (Gui::Algorithm::Random):  genAlgorithm = choosenAlgorithm; break;    
        
        case (Gui::Algorithm::None):    break;
        
        case (Gui::Algorithm::WallFollower):    solveAlgorithm = choosenAlgorithm; break;
        case (Gui::Algorithm::DepthFirstSearch):    solveAlgorithm = choosenAlgorithm; break;
        case (Gui::Algorithm::BreadthFirstSearch):  solveAlgorithm = choosenAlgorithm; break;
        case (Gui::Algorithm::DeadEndFiller):   solveAlgorithm = choosenAlgorithm; break;
        case (Gui::Algorithm::AStar):   solveAlgorithm = choosenAlgorithm; break;
    }
    
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
            ExportToGif(true);
        break;
        
        //!  Import
        case 7: //from  txt  or  json
            Import();
        break;
        
        default:
        break;
    }
    
    if(actionID >= 0 && actionID <= 6) {    // Export message
        popupMessage = "Exported Successfully!";
        popupTimer = 4.0f;   
    }
    
    if(popupTimer>0){
        DrawPopup();
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
    filename = TextFormat(".\\maze_%d.json", maze.Seed);
    bool success = SaveFileText(filename, (char *)mazeData.c_str());
    
    //export success
    if(success){
        TraceLog(LOG_INFO, TextFormat("Exported Successfully to %s!", filename));
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
            //corner
            bool cornerSolid = false;
            if (maze.grid[i][j].topWall) cornerSolid = true;                        
            if (maze.grid[i][j].leftWall) cornerSolid = true;                       
            if (i > 0 && maze.grid[i-1][j].leftWall) cornerSolid = true;            
            if (j > 0 && maze.grid[i][j-1].topWall) cornerSolid = true;             
            
            mazeData += cornerSolid ? "⬛" : "⬜";
            

            //topWall check
            if(maze.grid[i][j].topWall){
                mazeData += "⬛";
            }else if(includingSolution && SolutionBools[i][j] && (i == 0 || SolutionBools[i-1][j])){
                mazeData += "🟩";
            }else{
                mazeData += "⬜";
            }
        }
        //top right corner
        bool trCorner = false;
        if (maze.grid[i][maze.columns - 1].topWall) trCorner = true;                
        if (maze.grid[i][maze.columns - 1].rightWall) trCorner = true;              
        if (i > 0 && maze.grid[i-1][maze.columns - 1].rightWall) trCorner = true;   
        
        mazeData += trCorner ? "⬛\n" : "⬜\n";
        
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
        // bottom left corner
        bool blCorner = false;
        if (maze.grid[maze.rows-1][i].leftWall) blCorner = true;                    
        if (maze.grid[maze.rows-1][i].bottomWall) blCorner = true;                  
        if (i > 0 && maze.grid[maze.rows-1][i-1].bottomWall) blCorner = true;       
        
        mazeData += blCorner ? "⬛" : "⬜";

        
        // bottomWall check
        if(maze.grid[maze.rows-1][i].bottomWall){
            mazeData += "⬛";
        }else if(includingSolution && SolutionBools[maze.rows-1][i]){
            mazeData += "🟩"; 
        }else {
            mazeData += "⬜";
        }
    }
    //bottom-Right corner
    bool brCorner = false;
    if (maze.grid[maze.rows-1][maze.columns-1].rightWall) brCorner = true;          
    if (maze.grid[maze.rows-1][maze.columns-1].bottomWall) brCorner = true;        
    
    mazeData += brCorner ? "⬛" : "⬜";
    
    
    //exporting
    if(includingSolution){
        filename = TextFormat(".\\maze_%d_solution.txt", maze.Seed);
    }else{
        filename = TextFormat(".\\maze_%d.txt", maze.Seed);
    }
    bool success = SaveFileText(filename, (char *)mazeData.c_str());
    
    //export success
    if(success){
        TraceLog(LOG_INFO, TextFormat("Exported Successfully to %s!", filename));
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
    if(includingSolution){
        filename = TextFormat(".\\maze_%d_solution.png", maze.Seed);
    }else{
        filename = TextFormat(".\\maze_%d.png", maze.Seed);
    }
    bool success = ExportImage(exportImage, filename);
    
    //clean up
    UnloadImage(exportImage);
    UnloadRenderTexture(canvas);
    
    //export success
    if (success) {
        TraceLog(LOG_INFO, TextFormat("Exported Successfully to %s!", filename));
    } else {
        TraceLog(LOG_ERROR, "FAILURE: PNG Save failed.");
    }
}


int MazeExporter::CalculateGifDelay(bool alg) {
    // Determine which step counter to use based on the boolean
    int currentSteps = alg ? solveSteps : genSteps;
    if (currentSteps <= 0) currentSteps = 1;
    
    // 1. Calculate how many SECONDS the user wants the GIF to last
    float targetDurationSeconds = 60.0f * pow(0.5f, vSpeed - 1.0f);
    
    // 2. Lock the GIF playback to a smooth cinematic speed 
    int optimalDelay = 15; 
    float secondsPerGifFrame = optimalDelay / 100.0f;
    
    // 3. How many frames do we physically need to hit that target duration?
    int requiredFrames = targetDurationSeconds / secondsPerGifFrame;
    if (requiredFrames < 30) requiredFrames = 30;
    
    // 4. Calculate the exact frame skip to get exactly that many frames!
    this->frameSkip = currentSteps / requiredFrames;
    if (this->frameSkip < 1) this->frameSkip = 1; // You can't skip less than 1 frame
    
    // 5. Fallback for very small mazes (where steps are fewer than required frames)
    if (this->frameSkip == 1) {
        float adjustedSeconds = targetDurationSeconds / currentSteps;
        optimalDelay = (int)(adjustedSeconds * 100.0f);
        
        // Safe limits: 2 (50 FPS max) to 100 (1 FPS min)
        if (optimalDelay < 2) optimalDelay = 2; 
        if (optimalDelay > 100) optimalDelay = 100; 
    }
    
    return optimalDelay;
}

void MazeExporter::UpdateDrawContext() {
    //temporary GPU canvas
    RenderTexture2D canvas = LoadRenderTexture(drawContext.width, drawContext.height);
    
    //copy original drawContext making it centered on (0,0)
    Rectangle tempContext = {0, 0, drawContext.width, drawContext.height};
    
    BeginTextureMode(canvas);
    ClearBackground(BLANK);
    maze.Display(tempContext, {startRow, startCol}, {exitRow, exitCol});
    EndTextureMode();
    
    //save the image
    Image scoutImage = LoadImageFromTexture(canvas.texture);
    ImageFlipVertical(&scoutImage);
    
    //get boundaries
    Rectangle bounds = GetImageAlphaBorder(scoutImage, 0.0f);
    //update:
    drawContext.width = bounds.width;
    drawContext.height = bounds.height;
    
    //clean up
    UnloadImage(scoutImage);
    UnloadRenderTexture(canvas);
}

void MazeExporter::StartGif(int delay, int skipAmount) {
    if (isRecordingGif) return; // Prevent crashing 
    
    //update context
    UpdateDrawContext();
    
    // Set up the temporary camera
    gifCamera = LoadRenderTexture(drawContext.width, drawContext.height);
    frameSkip = skipAmount;
    frameCounter = 0;
    currentGifDelay = delay;
    isRecordingGif = true;
    
    // gif.h lib usage
    GifBegin((GifWriter*)gifRecorder, filename, drawContext.width, drawContext.height, delay);
}

void MazeExporter::WriteGifFrame(Maze &maze) {
    if (!isRecordingGif) return;    //prevent usage if StartGif() was not used before
    
    //frame skipping math
    frameCounter++;
    if (frameCounter % frameSkip != 0) return; 
    
    //copy original drawContext making it centered on (0,0)
    Rectangle tempContext = {0, 0, drawContext.width, drawContext.height};
    
    
    //save frame
    BeginTextureMode(gifCamera);
    ClearBackground(BLANK); 
    
    maze.Display(tempContext, {startRow, startCol}, {exitRow, exitCol});
    
    EndTextureMode();
    
    
    //pull from VRAM and proccess the image
    Image snapshot = LoadImageFromTexture(gifCamera.texture);
    ImageFlipVertical(&snapshot);
    //ImageAlphaCrop(&snapshot, 0.0f);
    
    
    
    //feed the raw pixels to the gif.h lib
    GifWriteFrame((GifWriter*)gifRecorder, (uint8_t*)snapshot.data, gifCamera.texture.width, gifCamera.texture.height, currentGifDelay);
    
    //clean up 
    UnloadImage(snapshot);
}

void MazeExporter::EndGif() {
    if (!isRecordingGif) return;    //prevent usage if StartGif() was not used before
    
    isRecordingGif = false;
    
    //final step: unloading texture and file export
    GifEnd((GifWriter*)gifRecorder);
    UnloadRenderTexture(gifCamera);
}

void MazeExporter::ExportToGif(bool includingSolution) {
    //calculate the delay based on Visualization Speed (vSpeed)
    int gifDelay = CalculateGifDelay(includingSolution);

    if(includingSolution){
        filename = TextFormat(".\\maze_%d_solution.gif", maze.Seed);
    }else{
        filename = TextFormat(".\\maze_%d.gif", maze.Seed);
    }

    //start the recorder using your new helper
    StartGif(gifDelay, frameSkip);
    
    //create new temporary maze
    Maze ghostMaze;
    if(includingSolution){
        ghostMaze = maze;
        ghostMaze.ClearSolution();
    }else{
        ghostMaze.CreateEmpty(maze.rows, maze.columns);
    }
    
    //use same seed
    SetRandomSeed(maze.Seed);
    ghostMaze.Seed = maze.Seed;
    
    if(includingSolution){
        // Init:
        switch (solveAlgorithm) {
            case (Gui::Algorithm::WallFollower):    WallFollower::Init(startRow, startCol, exitRow, exitCol, ghostMaze); break;
            case (Gui::Algorithm::DepthFirstSearch):    DepthFirstSearch::Init(startRow, startCol, exitRow, exitCol, ghostMaze); break;
            case (Gui::Algorithm::BreadthFirstSearch):  BreadthFirstSearch::Init(startRow, startCol, exitRow, exitCol, ghostMaze); break;
            case (Gui::Algorithm::DeadEndFiller):   DeadEndFiller::Init(startRow, startCol, exitRow, exitCol, ghostMaze); break;
            case (Gui::Algorithm::AStar):   AStar::Init(startRow, startCol, exitRow, exitCol, ghostMaze); break;
            default: break;
        }
        
        //Solving:
        while (!ghostMaze.Solved) {
            
            //take a step using correct function
            switch (solveAlgorithm) {
                case (Gui::Algorithm::WallFollower):    WallFollower::Solve(ghostMaze); break;
                case (Gui::Algorithm::DepthFirstSearch):    DepthFirstSearch::Solve(ghostMaze); break;
                case (Gui::Algorithm::BreadthFirstSearch):  BreadthFirstSearch::Solve(ghostMaze); break;
                case (Gui::Algorithm::DeadEndFiller):   DeadEndFiller::Solve(ghostMaze); break;
                case (Gui::Algorithm::AStar):   AStar::Solve(ghostMaze); break;
                default: break;
            }
            
            //saving snapshot
            WriteGifFrame(ghostMaze);
        }
    }else{
        // Init:
        switch (genAlgorithm) {
            case (Gui::Algorithm::Backtracking): Backtracking::Init(startRow, startCol, ghostMaze); break;
            case (Gui::Algorithm::HuntNKill):    HuntnKill::Init(startRow, startCol, ghostMaze); break;
            case (Gui::Algorithm::Prim):         Prim::Init(startRow, startCol, ghostMaze); break;
            case (Gui::Algorithm::Kruskal):      Kruskal::Init(ghostMaze); break;
            case (Gui::Algorithm::Eller):        Eller::Init(ghostMaze); break;
            case (Gui::Algorithm::Sidewinder):   Sidewinder::Init(ghostMaze); break;
            case (Gui::Algorithm::Random):       Random::Init(startRow, startCol, exitRow, exitCol, ghostMaze); break;
            default: break;
        }
        
        //Generation:
        while (!ghostMaze.Generated) {
            
            //take a step using the correct static class
            switch (genAlgorithm) {
                case (Gui::Algorithm::Backtracking): Backtracking::Generate(ghostMaze); break;
                case (Gui::Algorithm::HuntNKill):    HuntnKill::Generate(ghostMaze); break;
                case (Gui::Algorithm::Prim):         Prim::Generate(ghostMaze); break;
                case (Gui::Algorithm::Kruskal):      Kruskal::Generate(ghostMaze); break;
                case (Gui::Algorithm::Eller):        Eller::Generate(ghostMaze); break;
                case (Gui::Algorithm::Sidewinder):   Sidewinder::Generate(ghostMaze); break;
                case (Gui::Algorithm::Random):       Random::Generate(ghostMaze); break;
                default: break;
            }
            
            //saving snapshot
            WriteGifFrame(ghostMaze);
        }
        
    }
    this->frameSkip = 1;
    this->frameCounter = 0;
    
    // Final frames
    for (int i = 0; i < 10; i++){
        WriteGifFrame(ghostMaze);
    }
    ghostMaze.ChangeEveryCellColor(WHITE);
    for (int i = 0; i < 40; i++){
        WriteGifFrame(ghostMaze);
    }
    
    //export 
    EndGif();
}


void MazeExporter::Import() {
    //file filters
    const char* filterPatterns[2] = { "*.json", "*.txt" };

    //multiplatform dialog
    const char* selection = tinyfd_openFileDialog(
        "Select a Maze to Import",  // Title
        "./",                       // Default path
        2,                          // Number of filter patterns
        filterPatterns,             // The patterns
        "Maze Files",               // Description
        0                           // 0 = Single file only
    );

    //cancel button
    if (!selection) {
        popupMessage = "IMPORT CANCELED";
        popupTimer = 5.0f;
        return;
    }
    
    //choose correct parser
    string filePath = selection;
    if (filePath.find(".json") != string::npos) {
        ImportFromJson(filePath);
    } 
    else if (filePath.find(".txt") != string::npos) {
        ImportFromTxt(filePath);
    } 
    else {
        popupMessage = "UNSUPPORTED FORMAT!";
        popupTimer = 5.0f;
    }
}

void MazeExporter::ImportFromJson(string filepath) {
    ifstream file(filepath);
    
    if (!file.is_open()) {
        popupMessage = "ERROR: CANNOT OPEN FILE";
        popupTimer = 5.0f;
        return;
    }

    try {
        json mazeData = json::parse(file);
        file.close();

        if (!mazeData.contains("properties") || !mazeData.contains("grid")) {
            popupMessage = "ERROR: WRONG JSON FORMAT";
            popupTimer = 5.0f;
            return; //safely abort
        }

        int importedRows = mazeData["properties"]["rows"];
        int importedCols = mazeData["properties"]["columns"];
        
        startRow = (int)mazeData["properties"]["startRow"] - 1;
        startCol = (int)mazeData["properties"]["startCol"] - 1;
        exitRow  = (int)mazeData["properties"]["exitRow"] - 1;
        exitCol  = (int)mazeData["properties"]["exitCol"] - 1;

        maze.CreateEmpty(importedRows, importedCols);
        
        for (int r = 0; r < importedRows; r++) {
            for (int c = 0; c < importedCols; c++) {
                string walls = mazeData["grid"][r][c];
                maze.grid[r][c].rightWall  = (walls[0] == '1');
                maze.grid[r][c].leftWall   = (walls[1] == '1');
                maze.grid[r][c].topWall    = (walls[2] == '1');
                maze.grid[r][c].bottomWall = (walls[3] == '1');
            }
        }

        maze.Generated = true;
        maze.Solved = false;
        maze.ClearSolution();

        popupMessage = "JSON IMPORTED!";
        popupTimer = 5.0f;

    } catch (json::exception& e) {
        //any type of error catching
        popupMessage = "ERROR: CORRUPTED JSON";
        popupTimer = 5.0f;
    }
}

void MazeExporter::ImportFromTxt(string filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        popupMessage = "ERROR: CANNOT OPEN FILE";
        popupTimer = 5.0f;
        return;
    }

    string line;
    vector<vector<string>> gridEmojis;

    //read the text file and split the emojis into a 2D Array
    while (getline(file, line)) {
        if (line.empty()) continue;
        gridEmojis.push_back(SplitUTF8(line));
    }
    file.close();

    //maze file validation
    if (gridEmojis.size() < 3 || gridEmojis[0].size() < 3) {
        popupMessage = "ERROR: INVALID TXT FORMAT";
        popupTimer = 5.0f;
        return;
    }

    //calculate rows/cols (reversed 2N+1 expansion)
    int importedRows = (gridEmojis.size() - 1) / 2;
    int importedCols = (gridEmojis[0].size() - 1) / 2;

    maze.CreateEmpty(importedRows, importedCols);
    string wallEmoji = "⬛"; 

    //map the Emoji positions back to the Cell booleans
    for (int r = 0; r < importedRows; r++) {
        for (int c = 0; c < importedCols; c++) {
            //using precise grid math to find the walls surrounding the current cell
            maze.grid[r][c].topWall    = (gridEmojis[r * 2][c * 2 + 1] == wallEmoji);
            maze.grid[r][c].leftWall   = (gridEmojis[r * 2 + 1][c * 2] == wallEmoji);
            maze.grid[r][c].rightWall  = (gridEmojis[r * 2 + 1][c * 2 + 2] == wallEmoji);
            maze.grid[r][c].bottomWall = (gridEmojis[r * 2 + 2][c * 2 + 1] == wallEmoji);
        }
    }

    //setting exits as default
    startRow = 0;
    startCol = 0;
    exitRow = importedRows - 1;
    exitCol = importedCols - 1;

    maze.Generated = true; 
    maze.Solved = false;
    maze.ClearSolution();

    //popup
    popupMessage = "TXT IMPORTED!";
    popupTimer = 5.0f;
}

vector<string> MazeExporter::SplitUTF8(const string &str) {
    vector<string> result;
    size_t i = 0;
    while (i < str.length()) {
        int len = 1;
        unsigned char c = str[i];
        
        // Determine the byte-length of the character/emoji
        if (c >= 0xF0) len = 4;
        else if (c >= 0xE0) len = 3;
        else if (c >= 0xC0) len = 2;
        
        // Skip hidden carriage returns from Windows text files
        if (str[i] != '\n' && str[i] != '\r') {
            result.push_back(str.substr(i, len));
        }
        i += len;
    }
    return result;
}

void MazeExporter::DrawPopup() {
    if (popupTimer <= 0.0f) return;

    popupTimer -= GetFrameTime();

    float alpha = (popupTimer < 1.0f) ? popupTimer : 1.0f;

    int fontSize = 40;
    int textWidth = MeasureText(popupMessage.c_str(), fontSize);
    
    int x = GetScreenWidth() / 2 - textWidth / 2;
    int y = GetScreenHeight() / 2.2;

    // Draw a dark semi-transparent background box
    DrawRectangle(x - 20, y - 10, textWidth + 40, fontSize + 20, Fade(/*to*/BLACK, alpha * 0.8f));
    
    // Draw the text inside the box
    DrawText(popupMessage.c_str(), x, y, fontSize, Fade({27, 227, 84, 255}, alpha));
}