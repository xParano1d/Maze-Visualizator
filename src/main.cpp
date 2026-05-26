#include <iostream>
#include <raylib.h>
#include "Gui.h"
#include "MazeExporter.h"

#include "./gen/Backtracking.h"
#include "./gen/HuntnKill.h"
#include "./gen/Prim.h"
#include "./gen/Kruskal.h"
#include "./gen/Eller.h"
#include "./gen/Sidewinder.h"
#include "./gen/Random.h"

#include "./solve/WallFollower.h"
#include "./solve/DepthFirstSearch.h"
#include "./solve/BreadthFirstSearch.h"
#include "./solve/DeadEndFiller.h"
#include "./solve/AStar.h"


int main() {
    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(800, 600, "Maze Visualizator");
    
    //ICON:
    Image icon = LoadImage("./icon/icon.png");
    Image icon16 = ImageCopy(icon);
    ImageResize(&icon16, 16, 16);
    Image icon32 = ImageCopy(icon);
    ImageResize(&icon32, 32, 32);
    
    Image icons[3] = {icon, icon32, icon16};
    
    SetWindowIcons(icons, 3);
    UnloadImage(icon);
    UnloadImage(icon16);
    UnloadImage(icon32);
    SetExitKey(KEY_F4);

    int currentMonitor = GetCurrentMonitor();
    SetTargetFPS(GetMonitorRefreshRate(currentMonitor));

    
    //*---SETTINGS---*
    //*Screen:
    int screenWidth = GetMonitorWidth(currentMonitor) / 1.3;
    int screenHeight = GetMonitorHeight(currentMonitor) / 1.3;
    
    //*Maze:
    //Grid Size:
    int gridHeight = 10;    // rows
    int gridWidth = 10;     // columns
    
    //Start Position
    int startRow = 0;   //!DO NOT CHANGE: it should just be *0* for maze with top entrance and bottom exit
    int startCol = 0;
    
    //Exit Position
    int exitRow = gridHeight-1; //!DO NOT CHANGE: it should just be *gridHeight-1* for maze with top entrance and bottom exit
    int exitCol = gridWidth-1;
    
    //*Visualization:
    //Speed (in algorithm steps per second)
    float vSpeed = 10;  //(1-10) | higher value -> faster
    
    
    SetWindowSize(screenWidth, screenHeight);
    SetWindowPosition((GetMonitorWidth(currentMonitor) - screenWidth) / 2, (GetMonitorHeight(currentMonitor) - screenHeight) / 2);
    ClearWindowState(FLAG_WINDOW_HIDDEN);
    
    Maze maze;
    Gui gui(gridHeight, gridWidth, startCol, exitCol, vSpeed);
    MazeExporter exporter(maze, startRow, startCol, exitRow, exitCol, gui.CenterContext);

    double time;
    double dt;  //delta time
    double stepDelay;

    double genTime;
    double solveTime;
    bool algType;       // true -> Gen  |  false -> Solve

    int tempW = 0;
    int tempH = 0;
    float tempVSpeed = 0;
    bool RandomMaze = false;

    while (!WindowShouldClose()) {
        dt = GetFrameTime();
        if (dt > 0.1f){
            dt = 0.1f; 
        }

        if(gui.triggerInputSync) {
            gui.SyncInputs(maze.rows, maze.columns);
            gui.triggerInputSync = false;
        }

        if(tempH!=gridHeight || tempW!=gridWidth){
            tempW = gridWidth;
            tempH = gridHeight;

            exitRow = gridHeight - 1;

            maze.CreateEmpty(gridHeight, gridWidth);
            gui.ReadytoSolve = false;
            tempVSpeed = 0;
        }
        
        if(tempVSpeed!=vSpeed){
            float targetDuration = 60 * pow(0.5, vSpeed - 1);   //The "Halving" Formula
            stepDelay = targetDuration / (gridWidth*gridHeight);//Grid Size should be considered too
            tempVSpeed = vSpeed;
        }

        gui.UpdateMazeState(maze.Generated, maze.Solved);

        BeginDrawing();
        ClearBackground(BLACK);

        if(gui.ButtonsReadyToClick){
            gui.choosenAlgorithm = gui.MainButtonHandler();
            
            switch(gui.choosenAlgorithm){
                case (Gui::Algorithm::Backtracking): //DFS (gen)
                    maze.CreateEmpty(gridHeight, gridWidth);
                    Backtracking::Init(startRow, startCol, maze);
                    algType = true;
                break;
                
                case (Gui::Algorithm::HuntNKill):
                    maze.CreateEmpty(gridHeight, gridWidth);
                    HuntnKill::Init(startRow, startCol, maze);
                    algType = true;
                break;
                
                case (Gui::Algorithm::Prim):
                    maze.CreateEmpty(gridHeight, gridWidth);
                    Prim::Init(startRow, startCol, maze);
                    algType = true;
                break;
                
                case (Gui::Algorithm::Kruskal): //Randomized* Kruskal
                    maze.CreateEmpty(gridHeight, gridWidth);
                    Kruskal::Init(maze);
                    algType = true;
                break;
                
                case (Gui::Algorithm::Eller):
                    maze.CreateEmpty(gridHeight, gridWidth);
                    Eller::Init(maze);
                    algType = true;
                break;

                case (Gui::Algorithm::Sidewinder):
                    maze.CreateEmpty(gridHeight, gridWidth);
                    Sidewinder::Init(maze);
                    algType = true;
                break;

                case (Gui::Algorithm::Random):
                    maze.CreateEmpty(gridHeight, gridWidth);
                    Random::Init(startRow, startCol, exitRow, exitCol, maze);
                    algType = true;
                break;

                case (Gui::Algorithm::None):
                break;
                
                
                case (Gui::Algorithm::WallFollower):
                    WallFollower::Init(startRow, startCol, exitRow, exitCol, maze);
                    algType = false;
                break;
                
                case (Gui::Algorithm::DepthFirstSearch):
                    DepthFirstSearch::Init(startRow, startCol, exitRow, exitCol, maze);
                    algType = false;
                break;
                
                case (Gui::Algorithm::BreadthFirstSearch):  //Flood Fill
                    BreadthFirstSearch::Init(startRow, startCol, exitRow, exitCol, maze);
                    algType = false;
                break;
                
                case (Gui::Algorithm::DeadEndFiller):
                    DeadEndFiller::Init(startRow, startCol, exitRow, exitCol, maze);
                    algType = false;
                break;
                
                case (Gui::Algorithm::AStar):
                    AStar::Init(startRow, startCol, exitRow, exitCol, maze);
                    algType = false;
                break;
                
                // case (Gui::Algorithm::Tremaux):
                //     algType = false;
                // break;
            }
            
            if(!gui.ButtonsReadyToClick){
                if(algType){        //GENERATION
                    RandomMaze = (gui.choosenAlgorithm == Gui::Algorithm::Random);

                        if (startRow >= maze.rows) startRow = maze.rows - 1;
                        if (startCol >= maze.columns) startCol = maze.columns - 1;
                        if (exitRow >= maze.rows) exitRow = maze.rows - 1;
                        if (exitCol >= maze.columns) exitCol = maze.columns - 1;

                    gui.genIterations = 0;
                    gui.genIterations++;
                    
                    gui.genTime = 0;
                    genTime = GetTime();

                    maze.Seed = GetRandomValue(0, 9999999);
                    SetRandomSeed(maze.Seed);
                    
                }else{              //SOLVING
                    maze.ClearSolution();
                    gui.ImpossibleMessageVisible = false;
                    

                    gui.solveIterations = 0;
                    gui.solveIterations++;
                    
                    gui.solveTime = 0;
                    solveTime = GetTime();
                }
            }
            
        }else{
            if(gui.choosenAlgorithm != Gui::Algorithm::None){
                if(algType){
                    gui.genTime = GetTime()-genTime;
                }else{
                    gui.solveTime = GetTime()-solveTime;
                }
            }else{
                maze.highlightRowEnabled = false;
            }

            time += dt;

            while(time >= stepDelay){
                time -= stepDelay;
                switch (gui.choosenAlgorithm){
                    case (Gui::Algorithm::Backtracking):
                        Backtracking::Generate(maze);
                    break;
                    
                    case (Gui::Algorithm::HuntNKill):
                        HuntnKill::Generate(maze);
                    break;
                    
                    case (Gui::Algorithm::Prim):
                        Prim::Generate(maze);
                    break;
                    
                    case (Gui::Algorithm::Kruskal):
                        Kruskal::Generate(maze);
                    break;
                    
                    case (Gui::Algorithm::Eller):
                        Eller::Generate(maze);
                    break;

                    case (Gui::Algorithm::Sidewinder):
                        Sidewinder::Generate(maze);
                    break;

                    case (Gui::Algorithm::Random):
                        Random::Generate(maze);
                    break;


                    case (Gui::Algorithm::None):
                    break;
                    
                    
                    case (Gui::Algorithm::WallFollower):
                        if(gui.solveIterations>1){
                            WallFollower::Solve(maze);
                        }
                    break;
                    
                    case (Gui::Algorithm::DepthFirstSearch):
                        DepthFirstSearch::Solve(maze);
                    break;
                    
                    case (Gui::Algorithm::BreadthFirstSearch):
                        BreadthFirstSearch::Solve(maze);
                    break;
                    
                    case (Gui::Algorithm::DeadEndFiller):
                        DeadEndFiller::Solve(maze);
                    break;
                    
                    case (Gui::Algorithm::AStar):
                        AStar::Solve(maze);
                    break;
                    
                    // case (Gui::Algorithm::Tremaux):
                    //     maze.Solved = true;
                    // break;
                }
                
                //if        Generated          or            Solved          or            Impossible
                if((maze.Generated && algType) || (maze.Solved && !algType) || (maze.Impossible && !algType)){
                    gui.choosenAlgorithm = Gui::Algorithm::None;

                    if((GetTime()-genTime>1 && algType) || (GetTime()-solveTime>1 && !algType && !maze.Impossible)){
                        if(!RandomMaze){
                            maze.ChangeEveryCellColor(WHITE);
                            if(maze.Generated && algType  &&  gui.BraidBool){
                                maze.ConvertToBraid();
                            }
                        }
                        gui.ButtonsReadyToClick = true;
                        time = 0;
                        break;
                    }else if(maze.Impossible && !maze.Solved && !algType){
                        if(GetTime()-solveTime>2  && !algType){
                            gui.ButtonsReadyToClick = true;
                            gui.ImpossibleMessageVisible = false;
                            maze.Impossible = false;
                            time = 0;
                            break;
                        }else{
                            gui.ImpossibleMessageVisible = true;
                        }
                    }

                }else if(gui.choosenAlgorithm != Gui::Algorithm::None){
                    if(algType){
                        gui.genIterations++;
                        gui.genTime = GetTime()-genTime;
                    }else{
                        gui.solveIterations++;
                        gui.solveTime = GetTime()-solveTime;
                    }
                }
            }
        }

        maze.Display(gui.CenterContext, {startRow, startCol}, {exitRow, exitCol});
        gui.Display();
        gui.ReadytoSolve = maze.Generated;
        exporter.Update(gui.chosenFileExportAction, gui.choosenAlgorithm, vSpeed, gui.genIterations, gui.solveIterations);

        EndDrawing();
    }

    CloseWindow();
}