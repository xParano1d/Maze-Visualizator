#include <raylib.h>
#include "Gui.h"
#include "Grid.h"

int main() {

    constexpr int screenWidth = 1080;
    constexpr int screenHeight = 600;

    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    float aspectRatio = screenWidth / screenHeight;

    Grid grid;
    Gui gui;

    InitWindow(screenWidth, screenHeight, "Labyrinths Visualization");
    SetTargetFPS(60);

    gui.Init();
    grid.Create(60,60); //? Size of grid
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        switch (gui.Handle()){
            case (Gui::Algorithm::RecursiveBacktrack):
                // RecursiveBacktrack::Generate(0, 0);
                break;
            case (Gui::Algorithm::HuntNKill):
                
                break;
            case (Gui::Algorithm::Prim):
                
                break;
            case (Gui::Algorithm::Kruskal): 
                
                break;
        }
        gui.Display();




        grid.Display(centerX, centerY, centerX, centerX/aspectRatio);

        DrawText("Prosze polaczycv sie z nigga AI", screenWidth/3+10, 13, 20, GREEN); //! zapytanie o polaczenie z nigga ai

        EndDrawing();
    }

    CloseWindow();
}