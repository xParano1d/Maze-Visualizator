#include <raylib.h>
#include <vector>
using std::vector;
class Gui {
    private:
        Rectangle LeftContext;
        Rectangle CenterContext;
        Rectangle RightContext;
        struct Button{
            float posX;
            float posY;
            float width;
            float height;
            const char *text;
            Color buttonColor = BLUE;
            Color textColor = WHITE;
            void Display(){
                DrawRectangleRounded({posX, posY, width, height}, 0.3, 2, buttonColor);
                DrawText(text, posX+(width-(TextLength(text)*(width*0.1f))), posY+height*0.3f, width*0.1f, textColor);
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
        vector<Button> buttons;
        int s=0;
    public:
        float screenWidth;
        float screenHeight;
        void Init();
        void Display();
        void ChangeRectPosition(char c, float x, float y);
        Vector2 GetRectArea(char c);
        float GetRectPosX(char c);
        // float GetRectPosY(char c);
};