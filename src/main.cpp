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
    constexpr int screenWidth = 1080;
    constexpr int screenHeight = 600;

    //settings
    int gridWidth = 60;
    int gridHeight = 60;

    int startingRow = 0;
    int startingCol = 0;

    int exitRow = gridHeight-1;
    int exitCol = gridWidth-1;

    float vSpeed = 100000;     //visualization speed 
    vSpeed = 1 / vSpeed;    //heighest value -> faster
    



    Maze maze;
    Gui gui;

    Image icon = LoadImage("./icon/icon.png");
    InitWindow(screenWidth, screenHeight, "Labyrinths Visualization");
    // SetWindowMonitor(1);
    SetWindowIcon(icon);
    UnloadImage(icon);

    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL));
    

    double delay;
    double genTime;
    double solveTime;
    bool algType;       // true -> Gen  |  false -> solve

    gui.Init();
    maze.CreateEmpty(gridHeight, gridWidth);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        gui.Display();

        if(gui.ready){

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
            
            if(!gui.ready){
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
                delay = GetTime();
            }

        }else{
            
            if(GetTime()-delay > vSpeed){
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
                
                gui.solveReady = maze.Generated;
                
                //if        generated          or            solved
                if((maze.Generated && algType) || (maze.Solved && !algType)){
                    if(GetTime()-delay > 1){
                        maze.ChangeEveryCellColor(WHITE);
                        gui.choosenAlgorithm = Gui::Algorithm::None;
                        gui.ready = true;
                    }
                }else{
    
                    if(GetTime()-delay > vSpeed){
                        delay = GetTime();
                        if(algType){
                            gui.genIterations++;
                        }else{
                            gui.solveIterations++;
                        }
                    }
                    if(algType){
                        gui.genTime = GetTime() - genTime;
                    }else{
                        gui.solveTime = GetTime() - solveTime;
                    }
                }
            }
        }

        
        maze.Display();
        
        DrawText("Prosze polaczycv sie z nigga AI", screenWidth/3+10, 13, 20, GREEN); //! zapytanie o polaczenie z nigga ai
        EndDrawing();
    }

    CloseWindow();
}