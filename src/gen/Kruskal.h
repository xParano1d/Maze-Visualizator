#pragma once
#include "../Maze.h"

class Kruskal {
    private:
        inline static vector<Maze::Edge> edgeList;
    public:
        static void Init(Maze& maze);
        static void Generate(Maze& maze);
};