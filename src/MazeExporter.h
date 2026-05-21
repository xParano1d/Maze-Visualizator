#pragma once
#include "Maze.h"

class MazeExporter {
    private:
        Maze &maze;

        int &startRow;
        int &startCol;

        int &exitRow;
        int &exitCol;


        void ExportToJson();
        void ExportToTxt(bool includingSolution);
    
        void ExportToPng(bool includingSolution);
        void ExportToGif(bool includingSolution);

        void Import();

    public:
        MazeExporter(Maze &MazeToExport, int &startRow, int &startCol, int &exitRow, int &exitCol): maze(MazeToExport), startRow(startRow), startCol(startCol), exitRow(exitRow), exitCol(exitCol){};

        void Update(int actionID);
};