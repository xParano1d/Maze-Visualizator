#pragma once
#include "../Maze.h"

class Kruskal {
    private:
        
    public:
        inline static vector<Maze::Edge> edgeList;

        static void Init(Maze& maze);
        static void Generate(Maze& maze);
};