#include "Gui.h"
#include <raylib.h>
#include <filesystem>
#include <string>
#include <iostream>
using namespace std;
namespace fs = std::filesystem;

// Button b;

void Gui::Init() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    
    //Boxes settings
    float offsetX = screenWidth * 0.01;         // x4 on width
    float offsetY = screenHeight * 0.02;        // x2   on height
    float smallBoxWidth = screenWidth * 0.2;   // x2 on width
    float bigBoxWidth = screenWidth * 0.56;     // x1 on width
    float boxHeight = screenHeight * 0.96;      // x1   on height
    
    //Left Box
    LeftContext = {offsetX, offsetY, smallBoxWidth, boxHeight};
    //Center Box
    CenterContext = {2 * offsetX + smallBoxWidth, offsetY, bigBoxWidth, boxHeight};
    //Right Box
    RightContext = {screenWidth - (smallBoxWidth + offsetX), offsetY, smallBoxWidth, boxHeight};

    this->buttons.resize(1);
    this->buttons[0] = Button{offsetX+4, offsetY * 4, smallBoxWidth-8, offsetY*3,"Button Text"};
}

void Gui::Display() {
    
    DrawRectangleLinesEx(LeftContext, 2, WHITE);
    //todo topText, generateButton, "radio" checklist of algorythms
    // DrawText("Generation:", offsetX * 2, offsetY * 2, offsetX*2, WHITE);

    // string path = "./src/gen";
    // for (const auto & entry : fs::directory_iterator(path)){
    //     string filename = entry.path().filename().string();
    //     filename = filename.substr(0, filename.size() - 4);

    //     //todo vector of buttons resize another loop diplay names n shit 
    // }

    
    // if(this->buttons[0].IsClicked()){
    //     this->s++;
    // }
    
    // switch (this->s){
    //     case 1:
    //         this->buttons[0].ChangeColor(RED, WHITE);
    //         break;
    //     case 2:
    //         this->buttons[0].ChangeColor(GREEN, WHITE);
    //         break;
    //     default:
    //         if(this->s ==3){
    //             this->s = 0;
    //             this->buttons[0].ChangeColor(BLUE, WHITE);
    //         }
    //         break;
    // }

    if(this->buttons[0].IsHovered()){
        this->buttons[0].ChangeColor(WHITE, BLACK);
    }else{
        this->buttons[0].ChangeColor(GREEN, WHITE);
    }
    
    this->buttons[0].Display();


    DrawRectangleLinesEx(CenterContext, 2, WHITE);




    
    DrawRectangleLinesEx(RightContext, 2, WHITE);
}

void Gui::ChangeRectPosition(char c, float x, float y){
    switch (c){
    case 'l':
        this->LeftContext.x = x;
        this->LeftContext.y = y;
        break;
    case 'c':
        this->CenterContext.x = x;
        this->CenterContext.y = y;
        break;
    case 'r':
        this->RightContext.x = x;
        this->RightContext.y = y;
        break;
    }
}

Vector2 Gui::GetRectArea(char c){
    Vector2 point = {0};
    switch (c){
    case 'l':
        point.x = this->LeftContext.width;
        point.y = this->LeftContext.height;
        break;
    case 'c':
        point.x = this->CenterContext.width;
        point.y = this->CenterContext.height;
        break;
    case 'r':
        point.x = this->RightContext.width;
        point.y = this->RightContext.height;
        break;
    }
    return point;
}

float Gui::GetRectPosX(char c) {
    float x;
    switch (c){
    case 'l':
        x = this->LeftContext.x;
        break;
    case 'c':
        x = this->CenterContext.x;
        break;
    case 'r':
        x = this->RightContext.x;
        break;
    }
    return x;
}
