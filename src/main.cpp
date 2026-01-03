#include <iostream>
#include <raylib.h>
#include "Gui.h"

#include "./gen/Backtracking.h"
#include "./gen/HuntnKill.h"
#include "./gen/Prim.h"
#include "./gen/Kruskal.h"

#include "./solve/WallFollower.h"
#include "./solve/BreadthFirstSearch.h"
#include "./solve/DeadEndFiller.h"


int main() {
    constexpr int screenWidth = 1080;
    constexpr int screenHeight = 600;

    //settings
    int gridWidth = 10;
    int gridHeight = 10;

    int startingRow = 0;
    int startingCol = 0;

    float vSpeed = 30;     //visualization speed 
    vSpeed = 1 / vSpeed;    //heighest value -> faster
    



    Grid grid;
    Gui gui;

    Image icon = LoadImage("./icon.png");
    InitWindow(screenWidth, screenHeight, "Labyrinths Visualization");
    SetWindowIcon(icon);
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL));

    UnloadImage(icon);

    double delay;
    double genTime;
    double solveTime;
    bool algType;       // true -> Gen  |  false -> solve

    gui.Init();
    grid.Create(gridHeight, gridWidth);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        gui.Display();

        if(gui.ready){

            gui.choosenAlgorithm = gui.MainButtonHandler();

            switch(gui.choosenAlgorithm){
                case (Gui::Algorithm::Backtracking):
                    grid.Create(gridHeight, gridWidth);
                    Backtracking::Init(startingRow, startingCol, grid);
                    algType = true;
                break;
                
                case (Gui::Algorithm::HuntNKill):
                    grid.Create(gridHeight, gridWidth);
                    HuntnKill::Init(startingRow, startingCol, grid);
                    algType = true;
                break;
                
                case (Gui::Algorithm::Prim):
                    grid.Create(gridHeight, gridWidth);
                    Prim::Init(startingRow, startingCol, grid);
                    algType = true;
                break;
                
                case (Gui::Algorithm::Kruskal): //Randomized* Kruskal
                    grid.Create(gridHeight, gridWidth);
                    Kruskal::Init(grid);
                    gui.genIterations--;
                    algType = true;
                break;


                case (Gui::Algorithm::None):
                break;


                case (Gui::Algorithm::WallFollower):
                    WallFollower::Init(0, 0, grid);
                    algType = false;
                break;

                case (Gui::Algorithm::BreadthFirstSearch):
                    BreadthFirstSearch::Init(0, 0, grid);
                    algType = false;
                break;
                
                case (Gui::Algorithm::DeadEndFiller):
                    algType = false;
                    std::cout << "test" << std::endl;
                break;
                
                case (Gui::Algorithm::Dijkstra):
                    algType = false;
                break;
                
                case (Gui::Algorithm::AStar):
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
                    grid.ClearSolution();

                    gui.solveIterations = 0;
                    gui.solveIterations++;

                    gui.solveTime = 0;
                    solveTime = GetTime();
                }
                delay = GetTime();
            }

        }else{
            
            switch (gui.choosenAlgorithm){
                case (Gui::Algorithm::Backtracking):
                    if(GetTime()-delay > vSpeed){
                        if(!grid.generated){
                            Backtracking::Generate(grid);
                        }
                    }
                break;
                
                case (Gui::Algorithm::HuntNKill):
                    if(GetTime()-delay > vSpeed){
                        if(!grid.generated){
                            HuntnKill::Generate(grid);
                        }
                    }
                break;
               
                case (Gui::Algorithm::Prim):
                    if(GetTime()-delay > vSpeed){
                        if(!grid.generated){
                            Prim::Generate(grid);
                        }
                    }
                break;
                
                case (Gui::Algorithm::Kruskal):
                    if(GetTime()-delay > vSpeed){
                        if(!grid.generated){
                            Kruskal::Generate(grid);
                        }
                    }
                break;
                

                case (Gui::Algorithm::None):
                break;


                case (Gui::Algorithm::WallFollower):
                    if(GetTime()-delay > vSpeed){
                        if(!grid.Solved){
                            WallFollower::Solve(grid);
                        }
                    }
                break;

                case (Gui::Algorithm::BreadthFirstSearch):
                    if(GetTime()-delay > vSpeed){
                        if(!grid.Solved){
                            BreadthFirstSearch::Solve(grid);
                        }
                    }
                break;
                
                case (Gui::Algorithm::DeadEndFiller):
                    if(GetTime()-delay > vSpeed){
                        if(!grid.Solved){
                            DeadEndFiller::Solve(grid);
                        }
                    }
                break;
                    
                case (Gui::Algorithm::Dijkstra):
                    grid.Solved = true;
                break;
                
                case (Gui::Algorithm::AStar):  
                    grid.Solved = true;
                break;

                case (Gui::Algorithm::Tremaux):
                    grid.Solved = true;
                break;
            }
            

            gui.solveReady = grid.generated;
            
            //if        generated          or            solved
            if((grid.generated && algType) || (grid.Solved && !algType)){
                if(GetTime()-delay > vSpeed*3){
                    grid.ChangeEveryCellColor(WHITE);
                    gui.ready = true;
                    gui.choosenAlgorithm = Gui::Algorithm::None;
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
            }
            

            if(algType){
                gui.genTime = GetTime() - genTime;
            }else{
                gui.solveTime = GetTime() - solveTime;
            }
        }

        
        grid.Display();
        
        // DrawText("Prosze polaczycv sie z nigga AI", screenWidth/3+10, 13, 20, GREEN); //! zapytanie o polaczenie z nigga ai
        EndDrawing();
    }

    CloseWindow();
}