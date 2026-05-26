#pragma once
#include "Maze.h"
#include "Gui.h"

#include "./gen/Backtracking.h"
#include "./gen/HuntnKill.h"
#include "./gen/Prim.h"
#include "./gen/Kruskal.h"
#include "./gen/Eller.h"
#include "./gen/Sidewinder.h"
#include "./gen/Random.h"

#include "./solve/WallFollower.h"
#include "./solve/DepthFirstSearch.h"
#include "./solve/BreadthFirstSearch.h"
#include "./solve/DeadEndFiller.h"
#include "./solve/AStar.h"

#include <string>

class MazeExporter {
    private:
        Maze &maze;

        int &startRow;
        int &startCol;

        int &exitRow;
        int &exitCol;


        Rectangle drawContext;

        Gui::Algorithm genAlgorithm;
        Gui::Algorithm solveAlgorithm;

        float vSpeed;
        int genSteps;
        int solveSteps;

        const char* filename = TextFormat("./maze_%d", maze.Seed);

        void ExportToJson();
        void ExportToTxt(bool includingSolution);
    
        void ExportToPng(bool includingSolution);

        
        // --- Gif Export Stuff ---
        void ExportToGif(bool includingSolution);

        void* gifRecorder;
        RenderTexture2D gifCamera;

        bool isRecordingGif = false;
        int frameSkip = 1;
        int frameCounter = 0;
        int currentGifDelay = 2;

        int CalculateGifDelay(bool alg);    // true -> Gen  |  false -> Solve
        void UpdateDrawContext();

        void StartGif(int delay, int skipAmount);
        void WriteGifFrame(Maze& maze);
        void EndGif();
        // ------------------------


        void Import();
        void ImportFromJson(string filepath);
        void ImportFromTxt(string filepath);
        vector<string> SplitUTF8(const string &str);

        // pop-up text stuff
        float popupTimer = 0.0f;
        string popupMessage = "";
        void DrawPopup();

    public:
        MazeExporter(Maze &MazeToExport, int &startRow, int &startCol, int &exitRow, int &exitCol, Rectangle drawContext);
        ~MazeExporter();

        void Update(int actionID, Gui::Algorithm choosenAlgorithm, float vSpeed, int genSteps, int solveSteps);
};