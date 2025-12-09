#include <raylib.h>
#include "Gui.h"
#include "Grid.h"
#include "./gen/Backtracking.h"

#include <iostream>
using namespace std;

int main() {
    //settings
constexpr int screenWidth = 1080;
    constexpr int screenHeight = 600;
    int gridWidth = 20;
    int gridHeight = 20;
    float vSpeed = 0.05;                   //visualization speed (delay in seconds between iterations)


    Grid grid;
    Gui gui;

    

    InitWindow(screenWidth, screenHeight, "Labyrinths Visualization");
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL));
    
    double time;

    gui.Init();
    grid.Create(gridHeight, gridWidth);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        gui.Display();

        if(gui.readyGen){
            gui.genState = gui.GenHandle();

            switch(gui.genState){
                case (Gui::Algorithm::Backtracking):
                    grid.Create(gridHeight, gridWidth);
                    Backtracking::Init(0, 0, grid);

                    time = GetTime();
                    gui.readyGen = false;

                    cout << "Backtracking" << endl;
                break;
                
                case (Gui::Algorithm::HuntNKill):
                    cout << "HELLOOOO" << endl;
                break;
                
                case (Gui::Algorithm::Prim):
                    grid.Create(gridHeight, gridWidth);
                break;
                
                case (Gui::Algorithm::Kruskal):
                    cout << "time: "<< GetTime() << endl;
                break;
                
                case (Gui::Algorithm::None):
                break;
            }

        }else{

            switch (gui.genState){
                case (Gui::Algorithm::Backtracking):
                    if(GetTime()-time > vSpeed){
                        Backtracking::Generate(grid);
                        time = GetTime();
                    }else if(grid.stack.empty()){
                        gui.readyGen = true;
                        gui.genState = Gui::Algorithm::None;
                    }
                break;
                
                case (Gui::Algorithm::HuntNKill):
                    
                break;
               
                case (Gui::Algorithm::Prim):
                    
                break;
                
                case (Gui::Algorithm::Kruskal):
                    
                break;
                
                case (Gui::Algorithm::None):
                break;
            }

        }


        
        grid.Display();
        
        DrawText("Prosze polaczycv sie z nigga AI", screenWidth/3+10, 13, 20, BLUE); //! zapytanie o polaczenie z nigga ai
        EndDrawing();
    }

    CloseWindow();
}