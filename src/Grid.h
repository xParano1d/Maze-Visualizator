#include <vector>
#include <raylib.h>
using std::vector;
class Grid {
    public:
        struct Cell{
            bool rightWall;
            bool leftWall;
            bool topWall;
            bool bottomWall;
            Color color;
            bool visited = false;
        };
    
        void Create(int rows, int columns);
        void Display(float posX, float posY, float width, float height);

    private:
        int rows;
        int columns;
        vector<vector<Cell>> grid;
};