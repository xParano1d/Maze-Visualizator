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

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        ClearBackground(BLACK);
        
        gui.Display();

        grid.Create(60,60); //Size of grid
        grid.Display(centerX, centerY, centerX, centerX/aspectRatio);

        DrawText("Prosze polaczycv sie z nigga AI", screenWidth/3+10, 13, 20, GREEN); // zapytanie o polaczenie z nigga ai

        EndDrawing();
    }

    CloseWindow();
}