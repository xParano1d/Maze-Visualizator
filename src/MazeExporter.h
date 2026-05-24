#pragma once
#include "Maze.h"

class MazeExporter {
    private:
        Maze &maze;

        int &startRow;
        int &startCol;

        int &exitRow;
        int &exitCol;


        Rectangle &drawContext;

        float vSpeed;
        int genSteps;
        int solveSteps;

        void ExportToJson();
        void ExportToTxt(bool includingSolution);
    
        void ExportToPng(bool includingSolution);

        // --- Gif Export Stuff ---
        void ExportToGif(bool includingSolution);

        GifWriter gifRecorder;
        RenderTexture2D gifCamera;
        bool isRecordingGif = false;
        int frameSkip = 1;
        int frameCounter = 0;

        int CalculateGifDelay(bool alg);    // true -> Gen  |  false -> Solve

        void StartGif(int delay, int skipAmount);
        void WriteGifFrame(Maze& maze);
        void EndGif();
        // ------------------------


        void Import();

    public:
        MazeExporter(Maze &MazeToExport, int &startRow, int &startCol, int &exitRow, int &exitCol, Rectangle &drawContext): maze(MazeToExport), startRow(startRow), startCol(startCol), exitRow(exitRow), exitCol(exitCol), drawContext(drawContext) {};

        void Update(int actionID, float vSpeed, int genSteps, int solveSteps);
};