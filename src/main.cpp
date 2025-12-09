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
    // float vSpeed = 1;                   //visualization speed

    Grid grid;
    Gui gui;


    InitWindow(screenWidth, screenHeight, "Labyrinths Visualization");
    SetTargetFPS(60);
    SetRandomSeed((unsigned int)time(NULL));

    gui.Init();

    grid.Create(gridHeight, gridWidth);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        gui.Display();

        switch(gui.GenHandle()){
            case (Gui::Algorithm::Backtracking):
                grid.Create(gridHeight, gridWidth);

                Backtracking::Generate(0, 0, grid);

                gui.readyGen = true;
                cout << "done" << endl;
                break;
            case (Gui::Algorithm::HuntNKill):
                cout << "HELLOOOO" << endl;
                break;
            case (Gui::Algorithm::Prim):
                grid.Create(gridHeight, gridWidth);
                break;
            case (Gui::Algorithm::Kruskal):
                cout << "chuj: "<< endl;
                break;
            case (Gui::Algorithm::None):
                if(!gui.readyGen){
                    cout << "Generation busy. Please wait" << endl;
                }
                break;
        }
        
        grid.Display();
        
        DrawText("Prosze polaczycv sie z nigga AI", screenWidth/3+10, 13, 20, BLUE); //! zapytanie o polaczenie z nigga ai

        EndDrawing();
    }

    CloseWindow();
}