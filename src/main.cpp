#include <iostream>
#include <raylib.h>
#include "Gui.h"

#include "./gen/Backtracking.h"
#include "./gen/HuntnKill.h"
#include "./gen/Prim.h"
#include "./gen/Kruskal.h"
#include "./gen/Eller.h"

#include "./solve/WallFollower.h"
#include "./solve/DepthFirstSearch.h"
#include "./solve/BreadthFirstSearch.h"
#include "./solve/DeadEndFiller.h"
#include "./solve/AStar.h"


int main() {
    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(800, 600, "Labyrinths Visualization");
    Image icon = LoadImage("./icon/icon.png");
    // SetWindowMonitor(1);
    SetWindowIcon(icon);
    UnloadImage(icon);
    int currentMonitor = GetCurrentMonitor();
    
    SetTargetFPS(GetMonitorRefreshRate(currentMonitor));
    SetRandomSeed((unsigned int)time(NULL));
    
    //*---SETTINGS---*
    //*Screen:
    int screenWidth = GetMonitorWidth(currentMonitor) / 1.6;
    int screenHeight = GetMonitorHeight(currentMonitor) / 1.6;

    //*Maze:
    //Grid Size:
    int gridWidth = 10;     // rows
    int gridHeight = 10;    // columns

    //Starting Position for Solving
    int startingRow = 0;
    int startingCol = 0;

    //Exit 
    int exitRow = gridHeight-1;
    int exitCol = gridWidth-1;
    
    //*Visualization:
    //Speed (in algorithm steps per second)
    float vSpeed = 6;  //(1-10) | higher value -> faster

    
    SetWindowSize(screenWidth, screenHeight);
    SetWindowPosition((GetMonitorWidth(currentMonitor) - screenWidth) / 2, (GetMonitorHeight(currentMonitor) - screenHeight) / 2);
    ClearWindowState(FLAG_WINDOW_HIDDEN);

    Gui gui(gridHeight, gridWidth, vSpeed);
    Maze maze;

    double time;
    double dt;  //delta time
    double stepDelay;

    double genTime;
    double solveTime;
    bool algType;       // true -> Gen  |  false -> solve

    int tempW = 0;
    int tempH = 0;
    float tempVSpeed = 0;

    while (!WindowShouldClose()) {
        dt = GetFrameTime();
        if (dt > 0.1f){
            dt = 0.1f; 
        }
        
        if(tempH!=gridHeight || tempW!=gridWidth){
            tempW = gridWidth;
            tempH = gridHeight;
            
            exitRow = gridHeight-1;
            exitCol = gridWidth-1;
            
            maze.CreateEmpty(gridHeight, gridWidth);
            gui.ReadytoSolve = false;
            tempVSpeed = 0;
        }
        if(tempVSpeed!=vSpeed){
            float targetDuration = 60 * pow(0.5, vSpeed - 1);   //The "Halving" Formula
            stepDelay = targetDuration / (gridWidth*gridHeight);//Grid Size should be considered too
            tempVSpeed = vSpeed;
        }
        
        BeginDrawing();
        ClearBackground(BLACK);

        if(gui.ButtonsReadyToClick){
            gui.choosenAlgorithm = gui.MainButtonHandler();
            
            switch(gui.choosenAlgorithm){
                case (Gui::Algorithm::Backtracking): //DFS (gen)
                maze.CreateEmpty(gridHeight, gridWidth);
                Backtracking::Init(startingRow, startingCol, maze);
                algType = true;
                break;
                
                case (Gui::Algorithm::HuntNKill):
                maze.CreateEmpty(gridHeight, gridWidth);
                HuntnKill::Init(startingRow, startingCol, maze);
                algType = true;
                break;
                
                case (Gui::Algorithm::Prim):
                maze.CreateEmpty(gridHeight, gridWidth);
                Prim::Init(startingRow, startingCol, maze);
                algType = true;
                break;
                
                case (Gui::Algorithm::Kruskal): //Randomized* Kruskal
                maze.CreateEmpty(gridHeight, gridWidth);
                Kruskal::Init(maze);
                gui.genIterations--;
                algType = true;
                break;
                
                case (Gui::Algorithm::Eller):
                maze.CreateEmpty(gridHeight, gridWidth);
                Eller::Init(maze);
                gui.genIterations--;
                algType = true;
                break;
                
                
                case (Gui::Algorithm::None):
                break;
                
                
                case (Gui::Algorithm::WallFollower):
                WallFollower::Init(startingRow, startingCol, maze);
                algType = false;
                break;
                
                case (Gui::Algorithm::DepthFirstSearch):
                DepthFirstSearch::Init(startingRow, startingCol, exitRow, exitCol, maze);
                algType = false;
                break;
                
                case (Gui::Algorithm::BreadthFirstSearch):
                BreadthFirstSearch::Init(startingRow, startingCol, exitRow, exitCol, maze);
                algType = false;
                break;
                
                case (Gui::Algorithm::DeadEndFiller):
                DeadEndFiller::Init(startingRow, startingCol, exitRow, exitCol, maze);
                algType = false;
                break;
                
                case (Gui::Algorithm::AStar):
                AStar::Init(startingRow, startingCol, exitRow, exitCol, maze);
                algType = false;
                break;
                
                case (Gui::Algorithm::Tremaux):
                algType = false;
                break;
            }
            
            if(!gui.ButtonsReadyToClick){
                if(algType){        //GENERATION
                    gui.genIterations = 0;
                    gui.genIterations++;
                    
                    gui.genTime = 0;
                    genTime = GetTime();
                    
                }else{              //SOLVING
                    maze.ClearSolution();
                    
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
                    
                    
                    case (Gui::Algorithm::None):
                    break;
                    
                    
                    case (Gui::Algorithm::WallFollower):
                    WallFollower::Solve(maze);
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
                    
                    case (Gui::Algorithm::Tremaux):
                    maze.Solved = true;
                    break;
                }
                
                //if        generated          or            solved
                if((maze.Generated && algType) || (maze.Solved && !algType)){
                    gui.ReadytoSolve = maze.Generated;
                    gui.choosenAlgorithm = Gui::Algorithm::None;

                    if((GetTime()-genTime>1 && algType)||(GetTime()-solveTime>1 && !algType)){
                        maze.ChangeEveryCellColor(WHITE);
                        gui.ButtonsReadyToClick = true;

                        time = 0;
                        break;
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
        
        
        maze.Display();
        gui.Display();
        
        DrawText("Prosze polaczycv sie z nigga AI", screenWidth/3+10, 13, 20, GREEN); //! zapytanie o polaczenie z nigga ai
        EndDrawing();
    }

    CloseWindow();
}