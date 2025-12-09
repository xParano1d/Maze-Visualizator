#include "Gui.h"
#include <raylib.h>

void Gui::Init() {
    this->screenWidth = GetScreenWidth();
    this->screenHeight = GetScreenHeight();
    
    //Boxes settings
    this->offsetX = screenWidth * 0.01;         // x4 on width
    this->offsetY = screenHeight * 0.02;        // x2   on height
    float smallBoxWidth = screenWidth * 0.2;   // x2 on width
    float bigBoxWidth = screenWidth * 0.56;     // x1 on width
    float boxHeight = screenHeight * 0.96;      // x1   on height

    this->ChosenGen.alg = None;

    //Left Box
    LeftContext = {offsetX, offsetY, smallBoxWidth, boxHeight};

    this->genButtons.resize(4);    //? Number of Buttons for Generation Algorithms
    //Recursive Backtrack
    this->genButtons[0] = Button{offsetX+4, offsetY * 8, smallBoxWidth-8, offsetY*3,"Backtracking", Backtracking};
    //Hunt n' Kill
    this->genButtons[1] = Button{offsetX+4, offsetY * 11.5f, smallBoxWidth-8, offsetY*3,"Hunt n' Kill", HuntNKill};
    //Prim
    this->genButtons[2] = Button{offsetX+4, offsetY * 15, smallBoxWidth-8, offsetY*3,"Prim", Prim};
    //Kruskal
    this->genButtons[3] = Button{offsetX+4, offsetY * 18.5f, smallBoxWidth-8, offsetY*3,"Kruskal", Kruskal};

    //Generate Button
    this->StartGenButton = {offsetX+4, screenHeight-offsetY*5, smallBoxWidth-8, offsetY*3,"Start Generating"};



    //Center Box
    CenterContext = {2 * offsetX + smallBoxWidth, offsetY, bigBoxWidth, boxHeight};

    //Right Box
    RightContext = {screenWidth - (smallBoxWidth + offsetX), offsetY, smallBoxWidth, boxHeight};
}

Gui::Algorithm Gui::GenHandle() {
    
    if(StartGenButton.IsClicked() && readyGen){
        return ChosenGen.alg;
    }

    return None;
}

void Gui::Display() {
    DrawRectangleLinesEx(LeftContext, 2, WHITE);
    DrawText("Generation:", GetRectPosX(LEFT) + GetRectArea(LEFT).x /2 - MeasureText("Generation:", this->screenWidth*0.02)/2, GetRectPosY(LEFT) + this->offsetY, this->screenWidth*0.02, WHITE);
    
    for(Button btn : this->genButtons){
        if(btn.IsHovered()){                            //hovered
            btn.ChangeColor(RAYWHITE, BLACK);
        }else if(btn.text == ChosenGen.text){           //chosen
            btn.ChangeColor({3, 31, 143, 255}, WHITE);
        }else{                                          //rest
            btn.ChangeColor({143, 17, 28, 255}, WHITE);
        }

        btn.Display();

        if(btn.IsClicked()){
            this->ChosenGen = btn;
        }
    }

    if(StartGenButton.IsHovered()){
        StartGenButton.ChangeColor(RAYWHITE, BLACK);
    }else{
        StartGenButton.ChangeColor({143, 17, 28, 255}, WHITE);
    }
    StartGenButton.Display();
    
    DrawRectangleLinesEx(CenterContext, 2, WHITE);

    DrawRectangleLinesEx(RightContext, 2, WHITE);
}

void Gui::ChangeRectPosition(Context c, float x, float y){
    switch (c){
    case LEFT:
        this->LeftContext.x = x;
        this->LeftContext.y = y;
        break;
    case CENTER:
        this->CenterContext.x = x;
        this->CenterContext.y = y;
        break;
    case RIGHT:
        this->RightContext.x = x;
        this->RightContext.y = y;
        break;
    }
}

Vector2 Gui::GetRectArea(Context c){
    Vector2 point = {0};
    switch (c){
    case LEFT:
        point.x = this->LeftContext.width;
        point.y = this->LeftContext.height;
        break;
    case CENTER:
        point.x = this->CenterContext.width;
        point.y = this->CenterContext.height;
        break;
    case RIGHT:
        point.x = this->RightContext.width;
        point.y = this->RightContext.height;
        break;
    }
    return point;
}

float Gui::GetRectPosX(Context c) {
    float x;
    switch (c){
    case LEFT:
        x = this->LeftContext.x;
        break;
    case CENTER:
        x = this->CenterContext.x;
        break;
    case RIGHT:
        x = this->RightContext.x;
        break;
    }
    return x;
}

float Gui::GetRectPosY(Context c) {
    float y;
    switch (c){
    case LEFT:
        y = this->LeftContext.y;
        break;
    case CENTER:
        y = this->CenterContext.y;
        break;
    case RIGHT:
        y = this->RightContext.y;
        break;
    }
    return y;
}
