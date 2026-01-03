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


    this->genButtons.resize(6);    //? Number of Buttons for Generation Algorithms
    //Recursive Backtrack
    this->genButtons[0] = Button{GetRectPosX(LEFT)+12, offsetY * 8, smallBoxWidth-24, offsetY*3, "Backtracking", Backtracking};
    //Hunt n' Kill
    this->genButtons[1] = Button{GetRectPosX(LEFT)+12, offsetY * 11.5f, smallBoxWidth-24, offsetY*3, "Hunt n' Kill", HuntNKill};
    //Prim
    this->genButtons[2] = Button{GetRectPosX(LEFT)+12, offsetY * 15, smallBoxWidth-24, offsetY*3, "Prim", Prim};
    //Kruskal
    this->genButtons[3] = Button{GetRectPosX(LEFT)+12, offsetY * 18.5f, smallBoxWidth-24, offsetY*3, "Kruskal", Kruskal};
    //Eller
    this->genButtons[4] = Button{GetRectPosX(LEFT)+12, offsetY * 22, smallBoxWidth-24, offsetY*3, "Eller", Eller};
    //Sidewinder
    this->genButtons[5] = Button{GetRectPosX(LEFT)+12, offsetY * 25.5f, smallBoxWidth-24, offsetY*3, "Sidewinder", Sidewinder};

    //Generate Button
    this->StartGenButton = {GetRectPosX(LEFT)+8, screenHeight-offsetY*5, smallBoxWidth-16, offsetY*3, "Start Generating"};



    //Center Box
    CenterContext = {2 * offsetX + smallBoxWidth, offsetY, bigBoxWidth, boxHeight};
    //TODO: Settings button idk where




    //Right Box
    RightContext = {screenWidth - (smallBoxWidth + offsetX), offsetY, smallBoxWidth, boxHeight};


    this->solveButtons.resize(6);    //? Number of Buttons for Solve Algorithms
    //Hand On Wall 
    this->solveButtons[0] = Button{GetRectPosX(RIGHT)+12, offsetY * 8, smallBoxWidth-24, offsetY*3, "Wall Follower", WallFollower};
    //Breadth First Search
    this->solveButtons[1] = Button{GetRectPosX(RIGHT)+12, offsetY * 11.5f, smallBoxWidth-24, offsetY*3, "BreadthFirstSearch", BreadthFirstSearch};
    //DeadEndFiller
    this->solveButtons[2] = Button{GetRectPosX(RIGHT)+12, offsetY * 15, smallBoxWidth-24, offsetY*3, "Dead End Filler", DeadEndFiller};
    //Dijkstra
    this->solveButtons[3] = Button{GetRectPosX(RIGHT)+12, offsetY * 18.5f, smallBoxWidth-24, offsetY*3, "Dijkstra", Dijkstra};
    //A*
    this->solveButtons[4] = Button{GetRectPosX(RIGHT)+12, offsetY * 22, smallBoxWidth-24, offsetY*3, "A* (A Star)", AStar};
    //Trémaux
    this->solveButtons[5] = Button{GetRectPosX(RIGHT)+12, offsetY * 25.5f, smallBoxWidth-24, offsetY*3, "Trémaux", Tremaux};

    //Generate Button
    this->StartSolvingButton = {GetRectPosX(RIGHT)+8, screenHeight-offsetY*5, smallBoxWidth-16, offsetY*3, "Start Solving"};
}

Gui::Algorithm Gui::MainButtonHandler() {
    if(StartGenButton.IsClicked() && ready){
        ready = false;
        return ChosenGen.alg;
    }
    if(StartSolvingButton.IsClicked() && solveReady && ready){
        ready = false;
        return ChosenSolve.alg;
    }
    return None;
}

void Gui::Display() {
    DrawRectangleLinesEx(LeftContext, 2, WHITE);
    DrawText("Generation:", GetRectPosX(LEFT) + (GetRectArea(LEFT).x - MeasureText("Generation:", this->screenWidth*0.02))/2, GetRectPosY(LEFT) + this->offsetY, this->screenWidth*0.02, WHITE);
    
    for(Button btn : this->genButtons){
        if(btn.IsHovered()){                            //hovered
            if(btn.text == ChosenGen.text){
                btn.ChangeColor({27, 227, 84, 255}, WHITE);
            }else{
                btn.ChangeColor(RAYWHITE, BLACK);
            }
        }else if(btn.text == ChosenGen.text){           //chosen
            btn.ChangeColor({27, 227, 84, 255}, BLACK);
        }else{                                          //base color
            btn.ChangeColor({143, 17, 28, 255}, WHITE);
        }

        btn.Display();

        if(btn.IsClicked()){
            this->ChosenGen = btn;
        }
    }


    //Algorithm elapsed time and genIterations count display
    const char* elapsedGenTime = TextFormat("Time Elapsed: %.2f seconds", genTime);
    DrawText(elapsedGenTime, GetRectPosX(LEFT) + offsetX, this->screenHeight-(GetRectPosY(LEFT) + this->offsetY*5), this->screenWidth*0.006, WHITE);

    const char* genIterationsCount = TextFormat("Generation Step Count: %d", genIterations);
    DrawText(genIterationsCount, GetRectPosX(LEFT) + offsetX, this->screenHeight-(GetRectPosY(LEFT) + this->offsetY*6), this->screenWidth*0.006, WHITE);


    if(StartGenButton.IsHovered() && choosenAlgorithm == Algorithm::None){  //hovered allowed
        StartGenButton.ChangeColor(RAYWHITE, BLACK);
    }else if(choosenAlgorithm != Algorithm::None){                          //locked
        StartGenButton.ChangeColor({108, 117, 148, 255}, BLACK);
    }else{                                                                  //base color
        StartGenButton.ChangeColor({143, 17, 28, 255}, WHITE);
    }
    StartGenButton.Display();
    


    DrawRectangleLinesEx(CenterContext, 2, WHITE);



    DrawRectangleLinesEx(RightContext, 2, WHITE);
    DrawText("Solving:", GetRectPosX(RIGHT) + (GetRectArea(LEFT).x - MeasureText("Solving:", this->screenWidth*0.02))/2, GetRectPosY(RIGHT) + this->offsetY, this->screenWidth*0.02, WHITE);

    for(Button btn : this->solveButtons){
        if(btn.IsHovered() && solveReady){                      //hovered
            if(btn.text == ChosenSolve.text){
                btn.ChangeColor({27, 227, 84, 255}, WHITE);
            }else{
                btn.ChangeColor(RAYWHITE, BLACK);
            }
        }else if(btn.text == ChosenSolve.text && solveReady){  //chosen
            btn.ChangeColor({27, 227, 84, 255}, BLACK);
        }else{
            if(solveReady){
                btn.ChangeColor({46, 52, 230, 255}, WHITE);     //base color
            }else{
                btn.ChangeColor({108, 117, 148, 255}, BLACK);   //locked
            }
        }

        btn.Display();

        if(btn.IsClicked() && solveReady){
            this->ChosenSolve = btn;
        }
    }

    
    //Algorithm elapsed time and solveIterations count display
    const char* elapsedSolveTime = TextFormat("Time Elapsed: %.2f seconds", solveTime);
    DrawText(elapsedSolveTime, GetRectPosX(RIGHT) + offsetX, this->screenHeight-(GetRectPosY(RIGHT) + this->offsetY*5), this->screenWidth*0.01, WHITE);

    const char* solveIterationsCount = TextFormat("Solving Step Count: %d", solveIterations);
    DrawText(solveIterationsCount, GetRectPosX(RIGHT) + offsetX, this->screenHeight-(GetRectPosY(RIGHT) + this->offsetY*6), this->screenWidth*0.01, WHITE);


    if(StartSolvingButton.IsHovered() && choosenAlgorithm == Algorithm::None && solveReady){    //hovered allowed
        StartSolvingButton.ChangeColor(RAYWHITE, BLACK);
    }else if(choosenAlgorithm!=Algorithm::None || !solveReady){                                 //locked
        StartSolvingButton.ChangeColor({108, 117, 148, 255}, BLACK);
    }else{                                                                                      //base color
        StartSolvingButton.ChangeColor({46, 52, 230, 255}, WHITE);
    }
    StartSolvingButton.Display();
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
