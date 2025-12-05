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
    int gridWidth = 10;
    int gridHeight = 10;

    //variables 
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    float aspectRatio = screenWidth / screenHeight;
    bool updateGrid = true;

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
                RecursiveBacktrack::Generate(0, 0, grid, updateGrid);
                cout << "done" << endl;
                break;
            case (Gui::Algorithm::HuntNKill):
                cout << "HELLOOOO" << endl;
                break;
            case (Gui::Algorithm::Prim):
                break;
            case (Gui::Algorithm::Kruskal): 
                cout << grid.grid[0][0].visited << endl;
                break;
            case (Gui::Algorithm::None):
                break;
        }
        gui.Display();

        if(updateGrid){
            grid.Display(centerX, centerY, centerX, centerX / aspectRatio);
        }
        
        DrawText("Prosze polaczycv sie z nigga AI", screenWidth/3+10, 13, 20, GREEN); //! zapytanie o polaczenie z nigga ai

        EndDrawing();
    }

    CloseWindow();
}