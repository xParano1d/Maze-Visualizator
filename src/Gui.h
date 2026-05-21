#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma once
#include <raylib.h>
#include <vector>
#include <cmath>
using std::vector;

class Gui {
    private:
        int &mazeGridWidth;
        int &mazeGridHeight;

        int &mazeStartCol;
        int &mazeExitCol;

        float &visualizationSpeed;

        bool Maze_Generated;
        bool Maze_Solved;
    

        int tempCols;
        int tempStartC;
        int tempExitC;

        float offsetX;
        float offsetY;

        float smallBoxWidth;
        float bigBoxWidth;
        float boxHeight;

        enum Context {
            LEFT,
            CENTER,
            RIGHT
        };
        
        void ChangeRectPosition(Context c, float x, float y);
        void ResizeRect(Context c, float newWidth, float newHeight);

        Vector2 GetRectArea(Context c);
        float GetRectPosX(Context c);
        float GetRectPosY(Context c);

        void UpdateData();
        void UpdateContexts();

        void Init();

    public:
        void Display();

        Gui(int &gridHeight, int &gridWidth, int &startCol, int &exitCol, float &vSpeed)  : mazeGridWidth(gridWidth), mazeGridHeight(gridHeight), mazeStartCol(startCol), mazeExitCol(exitCol), visualizationSpeed(vSpeed) {
            Init();
        };

        enum Algorithm {
            // Gen
            Backtracking,
            HuntNKill,
            Prim,
            Kruskal,
            Eller,
            Sidewinder,
            Random,

            None,

            // Solve
            WallFollower,
            BreadthFirstSearch,
            DeadEndFiller,
            DepthFirstSearch,
            AStar,
            // Tremaux
        };
        Algorithm choosenAlgorithm;
        const char *GetAlgorithmName(Gui::Algorithm alg);

        double genTime = 0;
        int genIterations = 0;

        double solveTime = 0;
        int solveIterations = 0;

        bool ButtonsReadyToClick = true;
        bool ReadytoSolve = false;

        bool ImpossibleMessageVisible = false;
        Algorithm usedAlg;

        Algorithm MainButtonHandler();
        
        bool BraidBool;
        void UpdateMazeState(bool Generated, bool Solved);
        
        Rectangle CenterContext;
        int chosenFileExportAction = -1;
    private:    
        Rectangle LeftContext;
        Rectangle RightContext;

        int KeepFontSizeEven(float f);

        struct Control{
            float posX;
            float posY;
            float width;
            float height;

            bool IsHovered(){
                if((GetMouseX()>posX && GetMouseX()<posX+width) && (GetMouseY()>posY && GetMouseY()<posY+height)){
                    return true;
                }else{
                    return false;
                }
            }
            bool IsClicked(){
                if(IsHovered()){
                    if(IsMouseButtonPressed(0) || IsMouseButtonPressed(1)){
                        return true;
                    }
                }
                return false;
            }
        };
        struct Button : Control{
            const char *text;
            Algorithm alg;

            Color buttonColor = BLUE;
            Color textColor = WHITE;

            
            void Display(){
                DrawRectangleRounded({posX, posY, width, height}, 0.3, 3, buttonColor);
                int fontSize = round(height * 0.56f);
                if(fontSize%2!=0){
                    fontSize += fontSize % 2;
                }
                DrawText(text, posX+width/2 - MeasureText(text, fontSize)/2, posY+height*0.26f, fontSize, textColor);
            }
            void DisplayRectangle(Color textHighlight){
                DrawRectangle(posX, posY, width, height, buttonColor);
                int fontSize = round(height * 0.08f);
                if(fontSize%2!=0){
                    fontSize += fontSize % 2;
                }
                DrawText(text, posX+width/2 - MeasureText(text, fontSize)/2+4, posY+height*0.26f+4, fontSize, textColor);
                DrawText(text, posX+width/2 - MeasureText(text, fontSize)/2, posY+height*0.26f, fontSize, textHighlight);
            }
            void ChangeColor(Color buttonC, Color textC){
                buttonColor = buttonC;
                textColor = textC;
            }
            
        };
        bool AllButtonsLocked = false;
        
        vector<Button> genButtons;
        Button ChosenGen;
        Button StartGenButton;
        
        vector<Button> solveButtons;
        Button ChosenSolve;
        Button StartSolvingButton;
        
        Button MazeButton;
        

        struct InputBox : Control{
            private:
            Color textColor = WHITE;
            Color outline;
            char text[9] = "";

            int min=0;
            int max=0;
            int maxlength;
            bool Active;


            void Input(){    
                if(IsClicked() && !Active){
                    Active = true;
                }else if(((IsMouseButtonPressed(0) && !IsClicked())&& Active) || !IsWindowFocused()){
                    Active = false;
                }

                if(Active){
                    int textLength = TextLength(text);
                    int key = GetCharPressed();
                    if((key >= '0' && key <= '9') && textLength < maxlength){
                        //Appending
                        char tempStr[2] = { (char)key, '\0' };
                        TextAppend(text, tempStr, &textLength);
                        value = TextToInteger(text);
                        
                    }else if(IsKeyPressed(KEY_BACKSPACE)){
                        //deleting
                        if(textLength!=0){
                            text[textLength - 1] = '\0';
                            value = TextToInteger(text);
                        }
                    }
                }        
                //replace text with value
                if (!Active) {
                    TextCopy(text, TextFormat("%d", value));
                }
            }
            
            public:
            int value=min;
            bool correctValue = false;

            InputBox(int min=0, int max=0, int value=0, int maxLength=2, Color outline = BLACK){
                this->min = min;
                this->max = max;
                this->outline = outline;
                this->maxlength = maxLength;

                text[0] = '\0';
                this->value = value;
                Active = false;
            }

            void Update(int posX, int posY, int width, int height){
                this->posX = posX;
                this->posY = posY;
                this->width = width;
                this->height = height;
            }

            void Display(){
                Input();

                if(value >= min && value <= max){
                    correctValue = true;
                }else{
                    correctValue = false;
                }

                if(min!=max && !correctValue){
                    DrawRectangle(posX, posY, width, height, {143, 17, 28, 255});
                    textColor = WHITE;
                }else{
                    DrawRectangle(posX, posY, width, height, WHITE);
                    textColor = BLACK;
                }
                DrawRectangleLinesEx({posX, posY, width, height}, 2, outline);

                float fontSize = round(height - 2);
                int textWidth = MeasureText(text, fontSize);
                DrawText(text, posX + width / 2 - textWidth / 2, posY + 5, fontSize, textColor);
                
                //blinking cursor:
                if (Active && (int)(GetTime() * 2) % 2 == 0) {
                    DrawRectangle(posX + (width + textWidth)/2 + 2, posY + 5, 2, height - 10, BLACK);
                }
            }
        };

        int SettingsDescDisplayDelay;
        float SettingsFunctionsDelay;
        bool SettingsVisible = false;
        Button CloseSettings;

        
        void DisplayConfig(int fontSize);
        InputBox gridRowsInput;
        InputBox gridColumnsInput;

        InputBox startColInput;
        InputBox exitColInput;

        Button Braid;

        InputBox vSpeedInput;

        void DisplayFile(int fontSize);
        Button importGrid;
        vector<Button> exportGrid;
        vector<Button> exportSolution;
        

        void DisplayInfo(int fontSize);

        int chosenSettingsTab = 0;
        void DisplaySettingsWindow();
};