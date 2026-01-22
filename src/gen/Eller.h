#pragma once
#include "../Maze.h"

class Eller {
    private:
        inline static int row;
        inline static int columnIterator;
        inline static vector<int> groups;

    public:
        static void Init(Maze& maze);
        static void Generate(Maze& maze);
};