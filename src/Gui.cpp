#include "Gui.h"
#include <iostream>

const char* Gui::GetAlgorithmName(Gui::Algorithm alg) {
    switch (alg) {
        // --- GENERATION ---
        case Gui::Algorithm::Backtracking:       return "Backtracking";
        case Gui::Algorithm::HuntNKill:          return "Hunt n' Kill";
        case Gui::Algorithm::Prim:               return "Prim's Algorithm";
        case Gui::Algorithm::Kruskal:            return "Kruskal's Algorithm";
        case Gui::Algorithm::Eller:              return "Eller's Algorithm";
        case Gui::Algorithm::Sidewinder:         return "Sidewinder";
        case Gui::Algorithm::Random:             return "Random";
        
        // --- SOLVING ---
        case Gui::Algorithm::WallFollower:       return "Wall Follower";
        case Gui::Algorithm::DepthFirstSearch:   return "Depth First Search";
        case Gui::Algorithm::BreadthFirstSearch: return "Breadth First Search";
        case Gui::Algorithm::DeadEndFiller:      return "Dead End Filler";
        case Gui::Algorithm::AStar:              return "A* Pathfinding";
        // case Gui::Algorithm::Tremaux:            return "Tremaux";

        case Gui::Algorithm::None:               return "None";
        default:                                 return "Unknown";
    }
}

void Gui::UpdateData() {
    //Boxes settings
    this->offsetX = GetScreenWidth() * 0.01;         // x4 on width
    this->offsetY = GetScreenHeight() * 0.02;        // x2   on height
    this->smallBoxWidth = GetScreenWidth() * 0.2;   // x2 on width
    this->bigBoxWidth = GetScreenWidth() * 0.56;     // x1 on width
    this->boxHeight = GetScreenHeight() * 0.96;      // x1   on height
}

void Gui::UpdateContexts() {
    UpdateData();

    ChangeRectPosition(LEFT, offsetX, offsetY);
    ResizeRect(LEFT, smallBoxWidth, boxHeight);

    //*Center Box*
    ChangeRectPosition(CENTER, 2 * offsetX + smallBoxWidth, offsetY);
    ResizeRect(CENTER, bigBoxWidth, boxHeight);

    //*Right Box*
    ChangeRectPosition(RIGHT, GetScreenWidth() - (smallBoxWidth + offsetX), offsetY);
    ResizeRect(RIGHT, smallBoxWidth, boxHeight);
}

void Gui::Init() {
    UpdateContexts();

    float standardHeight = offsetY * 3;

    this->ChosenGen.alg = None;
    this->ChosenSolve.alg = None;

    //*Left Box*
    this->genButtons.resize(7);    //? Number of Buttons for Generation Algorithms
    //Recursive Backtrack
    this->genButtons[0] = Button{GetRectPosX(LEFT)+12, offsetY * 8, smallBoxWidth-24, standardHeight, "Backtracking", Backtracking};
    //Hunt n' Kill
    this->genButtons[1] = Button{GetRectPosX(LEFT)+12, offsetY * 11.5f, smallBoxWidth-24, standardHeight, "Hunt n' Kill", HuntNKill};
    //Prim
    this->genButtons[2] = Button{GetRectPosX(LEFT)+12, offsetY * 15, smallBoxWidth-24, standardHeight, "Prim", Prim};
    //Kruskal
    this->genButtons[3] = Button{GetRectPosX(LEFT)+12, offsetY * 18.5f, smallBoxWidth-24, standardHeight, "Kruskal", Kruskal};
    //Eller
    this->genButtons[4] = Button{GetRectPosX(LEFT)+12, offsetY * 22, smallBoxWidth-24, standardHeight, "Eller", Eller};
    //Sidewinder
    this->genButtons[5] = Button{GetRectPosX(LEFT)+12, offsetY * 25.5f, smallBoxWidth-24, standardHeight, "Sidewinder", Sidewinder};
    //Random
    this->genButtons[6] = Button{GetRectPosX(LEFT)+12, offsetY * 29, smallBoxWidth-24, standardHeight, "Random Noise", Random};


    //Generate Button
    this->StartGenButton = {GetRectPosX(LEFT)+8, GetScreenHeight()-offsetY*5, smallBoxWidth-16, standardHeight, "Start Generating"};
    

    
    //*Center Box*
    //Settings Button (covering whole Center Box):
    this->MazeButton = {GetRectPosX(CENTER), GetRectPosY(CENTER), bigBoxWidth, boxHeight, "\nPress Mouse Button\nto Change Settings!"};
    this->MazeButton.ChangeColor({27, 227, 84, 0}, BLACK);
    
    //Settings Menu section:
    this->gridRowsInput = InputBox(2, 100, mazeGridHeight, 3);
    this->gridColumnsInput = InputBox(2, 100, mazeGridWidth, 3);
    this->tempCols = mazeGridWidth;
    this->tempStartC = mazeStartCol;
    this->tempExitC = mazeExitCol;

    this->startColInput = InputBox(1, mazeGridWidth, mazeStartCol+1);
    this->exitColInput = InputBox(1, mazeGridWidth, mazeExitCol+1);

    this->Braid = Button{GetRectPosX(CENTER) + offsetX*11 + MeasureText("Delete Dead Ends:", KeepFontSizeEven(offsetX * 3)), offsetY*27.5f, offsetX * 9, offsetY * 3.5f, "False"};
    this->Braid.ChangeColor({143, 17, 28, 255}, WHITE);

    this->vSpeedInput = InputBox(1, 10, visualizationSpeed);

    this->SaveSettings = Button{GetRectPosX(CENTER)+(GetRectArea(CENTER).x-offsetX*12)/2, GetRectPosY(CENTER)+GetRectArea(CENTER).y-offsetY*5, offsetX*12, offsetY*4, "SAVE"};



    //*Right Box*
    this->solveButtons.resize(6);    //? Number of Buttons for Solve Algorithms
    //Hand On Wall 
    this->solveButtons[0] = Button{GetRectPosX(RIGHT)+12, offsetY * 8, smallBoxWidth-24, standardHeight, "Wall Follower", WallFollower};
    //Depth First Search
    this->solveButtons[1] = Button{GetRectPosX(RIGHT)+12, offsetY * 11.5f, smallBoxWidth-24, standardHeight, "DepthFirstSearch", DepthFirstSearch};
    //Breadth First Search
    this->solveButtons[2] = Button{GetRectPosX(RIGHT)+12, offsetY * 15, smallBoxWidth-24, standardHeight, "Flood Fill (BFS)", BreadthFirstSearch};
    //Dead End Filler
    this->solveButtons[3] = Button{GetRectPosX(RIGHT)+12, offsetY * 18.5f, smallBoxWidth-24, standardHeight, "Dead End Filler", DeadEndFiller};
    //A*
    this->solveButtons[4] = Button{GetRectPosX(RIGHT)+12, offsetY * 22, smallBoxWidth-24, standardHeight, "A* (A Star)", AStar};
    //Trémaux NOT IMPLEMENTED
    // this->solveButtons[5] = Button{GetRectPosX(RIGHT)+12, offsetY * 25.5f, smallBoxWidth-24, standardHeight, "Trémaux", Tremaux};

    //Generate Button
    this->StartSolvingButton = {GetRectPosX(RIGHT)+8, GetScreenHeight()-offsetY*5, smallBoxWidth-16, standardHeight, "Start Solving"};
}

Gui::Algorithm Gui::MainButtonHandler() {
    if(!AllButtonsLocked){
        if(StartGenButton.IsClicked() && ButtonsReadyToClick && ChosenGen.alg != None){
            ButtonsReadyToClick = false;
            return ChosenGen.alg;
        }
        if(StartSolvingButton.IsClicked() && ReadytoSolve && ButtonsReadyToClick && ChosenSolve.alg != None){
            ButtonsReadyToClick = false;
            return ChosenSolve.alg;
        }
    }
    return None;
}

int Gui::KeepFontSizeEven(float f){
    int i = round(f);
    if(i % 2 != 0){
        i++;
    }
    return i;
}

void Gui::Display() {
    //*LEFT CONTEXT*
    DrawRectangleLinesEx(LeftContext, 2, WHITE);
    DrawText("Generation:", GetRectPosX(LEFT) + (GetRectArea(LEFT).x - MeasureText("Generation:", GetScreenWidth()*0.02))/2, GetRectPosY(LEFT) + this->offsetY, KeepFontSizeEven(GetScreenWidth()*0.02), WHITE);
    
    for(Button btn : this->genButtons){
        if(btn.IsHovered() && !AllButtonsLocked){                            //hovered
            if(btn.text == ChosenGen.text){
                btn.ChangeColor({27, 227, 84, 255}, WHITE);
            }else{
                btn.ChangeColor(RAYWHITE, BLACK);
            }
        }else if(btn.text == ChosenGen.text && !AllButtonsLocked){           //chosen
            btn.ChangeColor({27, 227, 84, 255}, BLACK);
        }else{
            if(!AllButtonsLocked){                      //base color
                btn.ChangeColor({143, 17, 28, 255}, WHITE);
            }else{                                      //locked
                btn.ChangeColor({108, 117, 148, 255}, WHITE);
            }
        }

        btn.Display();

        if(btn.IsClicked() && !AllButtonsLocked){
            this->ChosenGen = btn;
        }
    }

    //Algorithm elapsed time and genIterations count display
    const char* elapsedGenTime = TextFormat("Time Elapsed: %.2f seconds", genTime);
    DrawText(elapsedGenTime, GetRectPosX(LEFT) + offsetX, GetScreenHeight()-(GetRectPosY(LEFT) + this->offsetY*5), KeepFontSizeEven(GetScreenWidth()*0.0106), WHITE);

    const char* genIterationsCount = TextFormat("Generation Step Count: %d", genIterations);
    DrawText(genIterationsCount, GetRectPosX(LEFT) + offsetX, GetScreenHeight()-(GetRectPosY(LEFT) + this->offsetY*6), KeepFontSizeEven(GetScreenWidth()*0.0106), WHITE);


    if(StartGenButton.IsHovered() && choosenAlgorithm == Algorithm::None && !AllButtonsLocked){  //hovered allowed
        StartGenButton.ChangeColor(RAYWHITE, BLACK);
    }else if(choosenAlgorithm != Algorithm::None || AllButtonsLocked){                          //locked
        StartGenButton.ChangeColor({108, 117, 148, 255}, BLACK);
    }else{                                                                                      //base color
        StartGenButton.ChangeColor({143, 17, 28, 255}, WHITE);
    }
    StartGenButton.Display();
    

    //*RIGHT CONTEXT*
    DrawRectangleLinesEx(RightContext, 2, WHITE);
    DrawText("Solving:", GetRectPosX(RIGHT) + (GetRectArea(LEFT).x - MeasureText("Solving:", GetScreenWidth()*0.02))/2, GetRectPosY(RIGHT) + this->offsetY, KeepFontSizeEven(GetScreenWidth()*0.02), WHITE);
    
    for(Button btn : this->solveButtons){
        if(btn.IsHovered() && ReadytoSolve){                          //hovered
            if(btn.text == ChosenSolve.text){
                btn.ChangeColor({27, 227, 84, 255}, WHITE);
            }else{
                btn.ChangeColor(RAYWHITE, BLACK);
            }
        }else if(btn.text == ChosenSolve.text && ReadytoSolve){       //chosen
            btn.ChangeColor({27, 227, 84, 255}, BLACK);
        }else{
            if(ReadytoSolve && !AllButtonsLocked){
                btn.ChangeColor({46, 52, 230, 255}, WHITE);      //base color
            }else{
                btn.ChangeColor({108, 117, 148, 255}, BLACK);    //locked
            }
        }
        
        btn.Display();
        
        if(btn.IsClicked() && ReadytoSolve){
            this->ChosenSolve = btn;
        }
    }
    
    //Algorithm elapsed time and solveIterations count display
    const char* elapsedSolveTime = TextFormat("Time Elapsed: %.2f seconds", solveTime);
    DrawText(elapsedSolveTime, GetRectPosX(RIGHT) + offsetX, GetScreenHeight()-(GetRectPosY(RIGHT) + this->offsetY*5), KeepFontSizeEven(GetScreenWidth()*0.0106), WHITE);
    
    const char* solveIterationsCount = TextFormat("Solving Step Count: %d", solveIterations);
    DrawText(solveIterationsCount, GetRectPosX(RIGHT) + offsetX, GetScreenHeight()-(GetRectPosY(RIGHT) + this->offsetY*6), KeepFontSizeEven(GetScreenWidth()*0.0106), WHITE);
    
    
    if(StartSolvingButton.IsHovered() && choosenAlgorithm == Algorithm::None && ReadytoSolve){    //hovered allowed
        StartSolvingButton.ChangeColor(RAYWHITE, BLACK);
    }else if(choosenAlgorithm!=Algorithm::None || !ReadytoSolve){                                 //locked
        StartSolvingButton.ChangeColor({108, 117, 148, 255}, BLACK);
    }else{                                                                                      //base color
        StartSolvingButton.ChangeColor({46, 52, 230, 255}, WHITE);
    }
    StartSolvingButton.Display();
    
    //*CENTER CONTEXT*
    DrawRectangleLinesEx(CenterContext, 2, WHITE);
    if(choosenAlgorithm == Algorithm::None){        //neither solving nor generating -> SETTINGS
        if(!SettingsVisible){
            if(MazeButton.IsHovered()){
                if(SettingsDescDisplayDelay == -1){
                    SettingsDescDisplayDelay = GetTime();
                }
                // for 3 seconds tooltip will be shown
                if(GetTime() - SettingsDescDisplayDelay < 3){
                    MazeButton.text = "\nPress Mouse Button\n to Change Settings!";
                    MazeButton.DisplayRectangle({27, 227, 84, 255});
                }
            }else{
                SettingsDescDisplayDelay = -1;
            }
            if(MazeButton.IsClicked()){
                AllButtonsLocked = true;
                SettingsFunctionsDelay = GetTime();
                SettingsVisible = true;
            }else{
                AllButtonsLocked = false;
            }
        }
        if(SettingsVisible){
            if(GetTime()-SettingsFunctionsDelay>0.01){
                DisplaySettingsWindow();
            }
        }
        if(ImpossibleMessageVisible){
            // Showing on screen ALgorithm that failed Solving the maze
            MazeButton.text = TextFormat("%s\n   IS UNABLE TO\nSOVLE THIS MAZE!", GetAlgorithmName(usedAlg));;
            MazeButton.DisplayRectangle(RED);
        }
    }else if(choosenAlgorithm != Algorithm::None){  //generating or solving -> CANCEL
        if(MazeButton.IsHovered()){
            if(SettingsDescDisplayDelay == -1){
                SettingsDescDisplayDelay = GetTime();
            }
            // for 2 seconds tooltip will be shown
            if(GetTime() - SettingsDescDisplayDelay < 2){
                MazeButton.text = "\nPress Mouse Button\n      to CANCEL!";
                MazeButton.DisplayRectangle(RED);
            }
        }else{
            SettingsDescDisplayDelay = -1;
        }
        if(MazeButton.IsClicked()){
            choosenAlgorithm = Algorithm::None;
            ButtonsReadyToClick = true;
        }
        if(!ImpossibleMessageVisible){
            usedAlg = ChosenSolve.alg;
        }
    }
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

void Gui::ResizeRect(Context c, float newWidth, float newHeight) {
    switch (c){
        case LEFT:
            this->LeftContext.width = newWidth;
            this->LeftContext.height = newHeight;
        break;
        
        case CENTER:
            this->CenterContext.width = newWidth;
            this->CenterContext.height = newHeight;
        break;
        
        case RIGHT:
            this->RightContext.width = newWidth;
            this->RightContext.height = newHeight;
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

void Gui::DisplaySettingsWindow() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(/*to*/BLACK, 0.9f)); //!METTALICA REFERENCE

    Vector2 centerDimmensions = GetRectArea(CENTER);
    DrawRectangle(GetRectPosX(CENTER), GetRectPosY(CENTER), centerDimmensions.x, centerDimmensions.y, {20, 22, 28, 255});
    DrawRectangleLinesEx(CenterContext, 2, WHITE);

    float fontSize = KeepFontSizeEven(offsetX * 3);
    float titlePosX = GetRectPosX(CENTER) + offsetX * 5;
    float descPosX = GetRectPosX(CENTER) + offsetX * 8;

    //Centered Window Title:
    DrawText("Settings", GetRectPosX(CENTER) + (GetRectArea(CENTER).x - MeasureText("Settings", fontSize))/2, GetRectPosY(CENTER) + offsetY*1.5f, fontSize, WHITE);

    DrawText("Maze:", titlePosX, offsetY*8, fontSize, WHITE);
    
    DrawText("Number of Rows:", descPosX, offsetY*11, fontSize, WHITE);
    gridRowsInput.Update(descPosX+MeasureText("Number of Columns:", fontSize)+offsetX*3, offsetY*10.5f, offsetX * 5.6f, offsetY * 3);
    gridRowsInput.Display();

    DrawText("Number of Columns:", descPosX, offsetY*14.5f, fontSize, WHITE);
    gridColumnsInput.Update(descPosX+MeasureText("Number of Columns:", fontSize)+offsetX*3, offsetY*14, offsetX * 5.6f, offsetY * 3);
    gridColumnsInput.Display();
    

    if(tempCols != gridColumnsInput.value || tempStartC != mazeStartCol || tempExitC != mazeExitCol){
        tempCols = gridColumnsInput.value;

        tempStartC = mazeStartCol;
        tempExitC = mazeExitCol;

        int colInputLimit=1;
        if(gridColumnsInput.value >= 10){
            colInputLimit++;
        }
        if(gridColumnsInput.value >= 100){
            colInputLimit++;
        }

        this->startColInput = InputBox(1, gridColumnsInput.value, tempStartC+1, colInputLimit);
        this->exitColInput = InputBox(1, gridColumnsInput.value, tempExitC+1, colInputLimit);
    }

    DrawText("Start Position:", descPosX, offsetY*20, fontSize, WHITE);
    startColInput.Update(descPosX+MeasureText("Start Position:", fontSize)+offsetX*3, offsetY*19.5f, offsetX * 5.6f, offsetY * 3);
    startColInput.Display();

    DrawText("Exit Position:", descPosX, offsetY*23.5f, fontSize, WHITE);
    exitColInput.Update(descPosX+MeasureText("Start Position:", fontSize)+offsetX*3, offsetY*23, offsetX * 5.6f, offsetY * 3);
    exitColInput.Display();

    DrawText("Delete Dead Ends:", descPosX, offsetY*28, fontSize, WHITE);
    if(Braid.IsClicked() && SettingsVisible){
        if(TextIsEqual(Braid.text, "False")){
            Braid.ChangeColor({27, 227, 84, 255}, WHITE);
            Braid.text = "True";
        }
        else if(TextIsEqual(Braid.text, "True")){
            Braid.ChangeColor({143, 17, 28, 255}, WHITE);
            Braid.text = "False";
        }
    }
    //TODO: Hover Braid Button
    Braid.Display();

    DrawText("Visualization Speed:", titlePosX, offsetY*35, fontSize, WHITE);
    DrawText("Steps per Second:", descPosX, offsetY*38.5f, fontSize, WHITE);
    vSpeedInput.Update(descPosX+MeasureText("Number of Columns:", fontSize)+offsetX*4, offsetY*38, offsetX * 4.6f, offsetY * 3);
    vSpeedInput.Display();

    bool correctInputs = false;
    if(gridRowsInput.correctValue && gridColumnsInput.correctValue && vSpeedInput.correctValue && startColInput.correctValue && exitColInput.correctValue){
        correctInputs = true;
    }

    //Save button:
    if(SaveSettings.IsHovered() && correctInputs){              //hovered
        SaveSettings.ChangeColor(RAYWHITE, BLACK);

    }else if(!correctInputs){                                   //locked
        SaveSettings.ChangeColor({143, 17, 28, 255}, BLACK);
        //tooltip
        DrawText("ERROR: INVALID INPUT WILL NOT BE SAVED!", GetRectPosX(CENTER) + (GetRectArea(CENTER).x - MeasureText("ERROR: INVALID INPUT WILL NOT BE SAVED!", fontSize*0.6f))/2, GetRectPosY(CENTER)+GetRectArea(CENTER).y-offsetY*7, fontSize*0.6f, {143, 17, 28, 255});

    }else{                                                       //base color
        SaveSettings.ChangeColor({27, 227, 84, 255}, WHITE);
    }
    SaveSettings.Display();

    //exiting settings
    if((SaveSettings.IsClicked() || IsKeyPressed(KEY_ESCAPE)) && correctInputs){
        SettingsVisible = false;
        
        //get values and change settings 
        mazeGridHeight = gridRowsInput.value;
        mazeGridWidth = gridColumnsInput.value;

        mazeStartCol = startColInput.value-1;
        mazeExitCol = exitColInput.value-1;

        visualizationSpeed = vSpeedInput.value;
        
        if(TextIsEqual(Braid.text, "False")){
            BraidBool = false;
        }else if(TextIsEqual(Braid.text, "True")){
            BraidBool = true;
        }

    }else if(IsKeyPressed(KEY_ESCAPE)){
        SettingsVisible = false;
    }
}