#include <raylib.h>
#include "Gui.h"
#include "Grid.h"
#include "./gen/RecursiveBacktrack.h"

#include <iostream>
using namespace std;

int main() {
    //settings
    constexpr int screenWidth = 1080;
    constexpr int screenHeight = 600;
    int gridWidth = 14;
    int gridHeight = 14;


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
        
        switch (gui.GenHandle()){
            case (Gui::Algorithm::RecursiveBacktrack):
                grid.Create(gridHeight, gridWidth);
                // updateGrid = false;
                RecursiveBacktrack::Generate(0, 0, grid);
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
                break;
            case (Gui::Algorithm::None):
                if(!gui.readyGen){
                    cout << "generation busy. please wait" << endl;
                }
                break;
        }
        gui.Display();

        grid.Display();
        
        DrawText("Prosze polaczycv sie z nigga AI", screenWidth/3+10, 13, 20, BLUE); //! zapytanie o polaczenie z nigga ai

        EndDrawing();
    }

    CloseWindow();
}