#include <raylib.h>
#include <vector>
using std::vector;
class Gui {
    private:
        enum Context {
            LEFT,
            CENTER,
            RIGHT
        };
        Rectangle LeftContext;
        Rectangle CenterContext;
        Rectangle RightContext;
        enum Algorithm {
            RecursiveBacktrack,
            HuntNKill,
            Prim,
            Kruskal
        };
        struct Button{
            float posX;
            float posY;
            float width;
            float height;
            const char *text;
            Algorithm alg;
            Color buttonColor = BLUE;
            Color textColor = WHITE;
            void Display(){
                DrawRectangleRounded({posX, posY, width, height}, 0.3, 3, buttonColor);
                float fontSize = height * 0.54f;
                DrawText(text, posX+width/2 - MeasureText(text, fontSize)/2, posY+height*0.26f, fontSize, textColor);
            }
            void ChangeColor(Color buttonC, Color textC){
                buttonColor = buttonC;
                textColor = textC;
            }
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
        vector<Button> genButtons;
        Button ChosenGen;
        Button StartGenButton;
        float offsetX;
        float offsetY;
    public:
        float screenWidth;
        float screenHeight;
        void Init();
        void Display();
        void ChangeRectPosition(Context c, float x, float y);
        Vector2 GetRectArea(Context c);
        float GetRectPosX(Context c);
        float GetRectPosY(Context c);
};