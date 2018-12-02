//
//  main.c
//  Final Project
// 
//  Created by Shant Haik, Gavin Neises, Jordan Wheeler
//

#include "raylib.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>

int BOARDSIZE = 9;
int BOMBAMOUNT = 1;
int POWERAMOUNT = 1;

// Fills the texture array
void FillTextureArray(int count, Texture2D * text) {
    const char * strings[] = {"images/empty.png", "images/one.png", "images/two.png", "images/three.png", 
                            "images/four.png", "images/five.png", "images/six.png", "images/seven.png", 
                            "images/eight.png", "images/bomb.png","images/flag.png","images/PowerUp.png",
                            "images/PowerDown.png","images/easy.png", "images/medium.png", "images/hard.png" };
    for(int i = 0; i < count;i++)
    {
        text[i] = LoadTexture(strings[i]);
    }
}

// Calculates the number shown for each tile
// This is depends on how many bombs is next to it
void FillBoard(Board * b) {
    for(int i = 0; i < BOARDSIZE; i++) {
        for(int j = 0; j < BOARDSIZE; j++) {
            if(b->elements[i][j] != BOMB && b->elements[i][j] != POWERUP && b->elements[i][j] != POWERDOWN) {
                int count = 0;
                //Checking top row
                if(j > 0) {
                    //Checking top left 
                    if(i > 0) {
                        if(b->elements[i - 1][j - 1] == BOMB) {
                            count++;
                        }
                    }
                    //Checking top middle
                    if(b->elements[i][j - 1] == BOMB) {
                        count++;
                    }
                    //Checking top right
                    if(i < BOARDSIZE - 1) {
                        if(b->elements[i + 1][j - 1] == BOMB) {
                            count++;
                        }
                    }
                }
                //Checking middle left 
                if(i > 0) {
                    if(b->elements[i - 1][j] == BOMB) {
                        count++;
                    }
                }
                //Checking middle right 
                if(i < BOARDSIZE - 1) {
                    if(b->elements[i + 1][j] == BOMB) {
                        count++;
                    }
                }
                //Checking bottom 
                if(j < BOARDSIZE - 1) {
                    if(i > 0) {
                        //Checking bottom left 
                        if(b->elements[i - 1][j + 1] == BOMB) {
                            count++;
                        }
                    }
                    //Checking bottom middle
                    if(b->elements[i][j + 1] == BOMB) {
                        count++;
                    }
                    //Checking bottom right 
                    if(i < BOARDSIZE - 1) {
                        if(b->elements[i + 1][j + 1] == BOMB) {
                            count++;
                        }
                    }
                }
                b->elements[i][j] = count;
            }
        }
    }
}

// Randomly places bombs, powerups, and powerdowns on the board
void RandomizeBoard(Board * b, int i, int j) {
    int bombCount = BOMBAMOUNT;
    int upCount = POWERAMOUNT;
    int downCount = POWERAMOUNT;
    
    //Adds bombs to board 
    while(bombCount != 0) {
        int x = GetRandomValue(0, BOARDSIZE - 1);
        int y = GetRandomValue(0, BOARDSIZE - 1);
        
        if(!(x == i && y == j) && b->elements[x][y] == EMPTY) {
            b->elements[x][y] = BOMB;
            bombCount--;
        }
    }
    
    //Adds Power Ups to Board 
    while(upCount != 0) {
        int x = GetRandomValue(0, BOARDSIZE - 1);
        int y = GetRandomValue(0, BOARDSIZE - 1);
        
        if(!(x == i && y == j) && b->elements[x][y] == EMPTY) {
            b->elements[x][y] = POWERUP;
            upCount--;
        }
    }
    
    //Adds Power Downs to Board 
    while(downCount != 0) {
        int x = GetRandomValue(0, BOARDSIZE - 1);
        int y = GetRandomValue(0, BOARDSIZE - 1);
        
        if(!(x == i && y == j) && b->elements[x][y] == EMPTY) {
            b->elements[x][y] = POWERDOWN;
            downCount--;
        }
    }
    
    FillBoard(b);
}

//
//  Handles what happens when an Up arrow is revealed (PowerUp)
//  REMOVEBOMB  -   At board->elements[x][y] where the PowerUp was found, make it = EMPTY
//                  Find a BOMB in the board and make board->elements[x][y] = EMPTY
//                  Call FillBoard to recalculate the numbers around each bomb
//                  Subtract 1 from BOMBAMOUNT
//  FLAGBOMB    -   At board->elements[x][y] where the PowerUp was found, make it = EMPTY
//                  Find a BOMB on the board and make flagged[x][y] = true
bool ApplyPowerUp(Board * board, bool flagged[BOARDSIZE][BOARDSIZE]) {
    
    PowerUp powerChoice = GetRandomValue(0, 1);
    int bombChoice = GetRandomValue(0, BOMBAMOUNT - 1);
    
    // Maps out all the bomb locations
    Map * bombMap = malloc(sizeof(Map[BOMBAMOUNT]));
    int count = 0;
    for(int i = 0;i<BOARDSIZE;i++)
    {
        for(int j = 0;j<BOARDSIZE;j++)
        {
            if(board->elements[i][j] == BOMB)
            {
                bombMap[count].x = i;
                bombMap[count].y = j;
                count++;
            }
        }
    }
    
    int x = bombMap[bombChoice].x;
    int y = bombMap[bombChoice].y;
    
    free(bombMap);
    
    if(powerChoice == REMOVEBOMB) // Removes a bomb from the board
    {
        board->elements[x][y] = EMPTY;
        BOMBAMOUNT--;
    }
    else if(powerChoice == FLAGBOMB) { // Adds a flag to a bomb location
        flagged[x][y] = true;
        return true;
    }
    return false;
}

//
//  Handles what happens when a Down arrow is revealed (PowerDown)
//  ADDBOMB     -   At board->elements[x][y] where the PowerDown was found, make it = EMPTY
//              -   Find a location in board->elements[x][y] that doesn't have a bomb and place a bomb there
//              -   Call FillBoard to recalculate the numbers around each bomb
//              -   Add 1 to BOMBAMOUNT
//  RESTART     -   At board->elements[x][y] where the PowerDown was found, make it = EMPTY
//                  make all elements in collided = false
bool ApplyPowerDown(Board * board, bool col[BOARDSIZE][BOARDSIZE]) {
    
    PowerDown powerChoice = GetRandomValue(0, 1);
    
    int emptyCount = 0;
    for(int i = 0;i<BOARDSIZE;i++) // Gets the amount of spaces for a bomb to be placed in
    {
        for(int j = 0;j<BOARDSIZE;j++)
        {
            if(!(board->elements[i][j] == POWERUP || board->elements[i][j] == POWERDOWN || board->elements[i][j] == BOMB))
            {
                emptyCount++;
            }
        }
    }
    
    int emptyChoice = GetRandomValue(0, emptyCount - 1);
    
    Map emptyMap[emptyCount];
    int count = 0;
    for(int i = 0;i<BOARDSIZE;i++) // 
    {
        for(int j = 0;j<BOARDSIZE;j++)
        {
            if(!(board->elements[i][j] == POWERUP || board->elements[i][j] == POWERDOWN || board->elements[i][j] == BOMB))
            {
                emptyMap[count].x = i;
                emptyMap[count].y = j;
                count++;
            }
        }
    }
    
    int x = emptyMap[emptyChoice].x;
    int y = emptyMap[emptyChoice].y;
    
    if(powerChoice == ADDBOMB) {
        col[x][y] = false;
        board->elements[x][y] = BOMB;
        BOMBAMOUNT++;
        return true;
    }
    else if (powerChoice == REFRESH) { // Makes entire board show fog of war
        for(int i = 0;i<BOARDSIZE;i++)
        {
            for(int j = 0;j<BOARDSIZE;j++)
            {
                col[i][j] = false;
            }
        }
    }
    return false;
}

// Checks the 8 surrounding tiles of the move that was
// placed to determine if they should be shown or not
void CheckSurroundingSpaces(int i, int j, bool col[BOARDSIZE][BOARDSIZE], Board * b) {
    //Checking top row
    if(j > 0) {
        //Checking top left 
        if(i > 0) {
            if((b->elements[i - 1][j - 1] != BOMB) && (b->elements[i - 1][j - 1] != POWERUP) && (b->elements[i - 1][j - 1] != POWERDOWN)) {
                col[i - 1][j - 1] = true;
            }
        }
        //Checking top middle
        if((b->elements[i][j - 1] != BOMB) && (b->elements[i][j - 1] != POWERUP) && (b->elements[i][j - 1] != POWERDOWN)) {
            col[i][j - 1] = true;
        }
        //Checking top right
        if(i < 8) {
            if((b->elements[i + 1][j - 1] != BOMB) && (b->elements[i + 1][j - 1] != POWERUP) && (b->elements[i + 1][j - 1] != POWERDOWN)) {
                col[i + 1][j - 1] = true;
            }
        }
    }
    //Checking middle left 
    if(i > 0) {
        if((b->elements[i - 1][j] != BOMB) && (b->elements[i - 1][j] != POWERUP) && (b->elements[i - 1][j] != POWERDOWN)) {
            col[i - 1][j] = true;
        }
    }
    //Checking middle right 
    if(i < 8) {
        if((b->elements[i + 1][j] != BOMB) && (b->elements[i + 1][j] != POWERUP) && (b->elements[i + 1][j] != POWERDOWN)) {
            col[i + 1][j] = true;
        }
    }
    //Checking bottom 
    if(j < 8) {
        if(i > 0) {
            //Checking bottom left 
            if((b->elements[i - 1][j + 1] != BOMB) && (b->elements[i - 1][j + 1] != POWERUP) && (b->elements[i - 1][j + 1] != POWERDOWN)) {
                col[i - 1][j + 1] = true;
            }
        }
        //Checking bottom middle
        if((b->elements[i][j + 1] != BOMB) && (b->elements[i][j + 1] != POWERUP) && (b->elements[i][j + 1] != POWERDOWN)) {
            col[i][j + 1] = true;
        }
        //Checking bottom right 
        if(i < 8) {
            if((b->elements[i + 1][j + 1] != BOMB) && (b->elements[i + 1][j + 1] != POWERUP) && (b->elements[i + 1][j + 1] != POWERDOWN)) {
                col[i + 1][j + 1] = true;
            }
        }
    }
}

// Adds all the Rectangle objects for the board,
// Rectangle objects are necessary for registering
// the mouse clicked in a specific spot.
void AddTiles(Rectangle tiles[BOARDSIZE][BOARDSIZE]) {
    
    int xStart = 15;
    int yStart = 15;
    
    for(int i = 0;i < BOARDSIZE;i++)
    {
        for(int j = 0;j < BOARDSIZE;j++)
        {
            tiles[i][j].x = xStart;
            tiles[i][j].y = yStart;
            tiles[i][j].width = 15;
            tiles[i][j].height = 15;
            xStart+=16;
        }
        xStart = 15;
        yStart+=16;
    }
}

// Shows initial window asking to choose difficulty
// If nothing is chosen, the system defaults to EASY
void GetInitialSize() {
    int screenWidth = 153;
    int screenHeight = 100;
    
    Rectangle buttons[3];
    int xButtonStart = 5;
    int yButtonStart = 5;
    for(int i = 0;i<3;i++)
    {
        buttons[i].x = xButtonStart;
        buttons[i].y = yButtonStart;
        buttons[i].width = 47;
        buttons[i].height = 30;
        xButtonStart+=48;
    }
    
    Vector2 mouseLocation;

    InitWindow(screenWidth, screenHeight, "Spiked Ball Sweeper");
    
    Font ourFont = LoadFontEx("fonts/comic.ttf", 20, 0, NULL);
    
    int textureCount = 16;
    Texture2D * textures = malloc(textureCount*sizeof(Texture2D));
    FillTextureArray(textureCount, textures);
    
    SetTargetFPS(60);
    
    bool sizeChosen = false;
    
    while (!WindowShouldClose() && !sizeChosen)
    {
        mouseLocation = GetMousePosition();
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        for(int i = 0;i<3;i++)
        {
            DrawRectangleRec(buttons[i],WHITE);
            DrawTexture(textures[13+i],buttons[i].x, buttons[i].y,WHITE);
        }
        
        Vector2 textLocation;
        textLocation.x = 5;
        textLocation.y = screenHeight-55;
        
        // font, Text, Vector2, fontSize, spacing, Color
        Message text;
        text.m = "Please choose a \ndifficulty option.";
        DrawTextEx(ourFont,text.m,textLocation, 20, 1, BLACK);
        
        if(CheckCollisionPointRec(mouseLocation, buttons[0]))
        {
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                BOARDSIZE = 9;
                BOMBAMOUNT = 9;
                POWERAMOUNT = 1;
                sizeChosen = true;
            }
        }
        else if(CheckCollisionPointRec(mouseLocation, buttons[1]))
        {
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                BOARDSIZE = 16;
                BOMBAMOUNT = 40;
                POWERAMOUNT = 2;
                sizeChosen = true;
            }
        }
        else if(CheckCollisionPointRec(mouseLocation, buttons[2]))
        {
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                BOARDSIZE = 24;
                BOMBAMOUNT = 90;
                POWERAMOUNT = 3;
                sizeChosen = true;
            }
        }
        
        EndDrawing();
    } 
    CloseWindow();
    
    // Unloads and frees memory for items that were loaded or malloc'ed
    for(int i = 0;i<textureCount;i++)
    {
        UnloadTexture(textures[i]);
    }
    free(textures);
    UnloadFont(ourFont);
}

int main() {
    
    // Asks the user what difficulty to use
    GetInitialSize();
    
    int screenWidth = (16*BOARDSIZE)+30;
    int screenHeight = (16*BOARDSIZE)+80;
    
    InitWindow(screenWidth, screenHeight, "Spiked Ball Sweeper");
    
    Font ourFont = LoadFontEx("fonts/comic.ttf", 20, 0, NULL);
    
    int textureCount = 16;
    Texture2D * textures = malloc(textureCount*sizeof(Texture2D));
    FillTextureArray(textureCount, textures);
    
    // Fills the array holding the Rectangles required for collision detection.
    // Each element in tiles represents a square on the board.
    Rectangle tiles[BOARDSIZE][BOARDSIZE];
    
    AddTiles(tiles);
    
    bool collided[BOARDSIZE][BOARDSIZE];                            // Initializing collided
    bool flagged[BOARDSIZE][BOARDSIZE];                             // Initializing flagged
	Board * board = malloc(sizeof(Piece[BOARDSIZE][BOARDSIZE]));    // Initializing board
    
    // Setting initial conditions of collided, flagged and board
    for(int i = 0;i < BOARDSIZE;i++)
    {
        for(int j = 0;j < BOARDSIZE;j++)
        {
            collided[i][j] = false;
            flagged[i][j] = false;
            board->elements[i][j] = EMPTY;
        }
    }
    
    Vector2 mouseLocation;
    
    SetTargetFPS(15);
    
    bool firstClick = true;
    
    Status status = PENDING;
    
    int flagOnBombCount = 0;
    int flagCount = 0;
    
    Message textTop;
    Message textBot;
    textTop.m = "Welcome to:";// \nSpiked Ball Sweeper!";
    textBot.m = "Spiked Ball Sweeper!";
    
    Vector2 textLocation;
    textLocation.y = screenHeight-55;
    
    // Renews the display continuously until the program is closed or Esc key is pressed.
    while (!WindowShouldClose())
    {
        // Pressing Spacebar shows the board and instantly loses the game
        // Used for testing purposes
        if(IsKeyPressed(KEY_SPACE))
        {
            for(int i = 0;i<BOARDSIZE;i++)
            {
                for(int j = 0;j<BOARDSIZE;j++)
                {
                    collided[i][j] = true;
                    flagged[i][j] = false;
                }
            }
        }
        
        // Gets the Vector2 location of the mouses current position
        mouseLocation = GetMousePosition();
        
        // Prints the first line of our text display
        textLocation.x = (screenWidth/2) - MeasureTextEx(ourFont, textTop.m, ourFont.baseSize*2, 1).x/4;
        textLocation.y = screenHeight-55;
        DrawTextEx(ourFont,textTop.m,textLocation, 20, 1, BLACK);
        
        // Prints the second line of our text display
        textLocation.x = (screenWidth/2) - MeasureTextEx(ourFont, textBot.m, ourFont.baseSize*2, 1).x/4;
        textLocation.y = screenHeight-35;
        DrawTextEx(ourFont,textBot.m,textLocation, 20, 1, BLACK);
        
        // Determines if the game is won and shows the Won screen
        if(status == WON)
        {
            for(int i = 0;i<BOARDSIZE;i++) {
                for(int j = 0;j<BOARDSIZE;j++) {
                    if(board->elements[i][j] == BOMB) {
                        board->elements[i][j] = FLAG;
                    }
                    else if(board->elements[i][j] == POWERDOWN || board->elements[i][j] == POWERUP)
                    {
                        board->elements[i][j] = EMPTY;
                    }
                    collided[i][j] = true;
                }
            }
            textTop.m = "You Won!"; 
            textBot.m = "You got gud.";
        }
        // Shows Loss Screen
        else if(status == LOST)
        {
            for(int i = 0;i<BOARDSIZE;i++) {
                for(int j = 0;j<BOARDSIZE;j++) {
                    collided[i][j] = true;
                }
            }
            textTop.m = "You hit a bomb.";
            textBot.m = "You lose, git gud.";
        }
        // Collision detection for the first play
        else if (firstClick)
        {
            for(int i = 0;i<BOARDSIZE;i++) {
                for(int j = 0;j<BOARDSIZE;j++) {
                    if(CheckCollisionPointRec(mouseLocation, tiles[i][j]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        RandomizeBoard(board, i, j);
                        firstClick = false;
                        collided[i][j] = true;
                    }
                }
            }
        }
        // Collision detection for all plays that are not the first 
        else if(status == PENDING)
        {
            for(int i = 0;i<BOARDSIZE;i++) {
                for(int j = 0;j<BOARDSIZE;j++) {
                    if(CheckCollisionPointRec(mouseLocation, tiles[i][j]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && (board->elements[i][j] == EMPTY || collided[i][j] == false)) {
                        if(!flagged[i][j])
                        {
                            collided[i][j] = true;
                            CheckSurroundingSpaces(i, j, collided, board);
                        }
                    }
                    else if(CheckCollisionPointRec(mouseLocation, tiles[i][j]) && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                        if(!flagged[i][j]) {
                            if(board->elements[i][j] == BOMB) {
                                flagged[i][j] = true;
                            }
                            else {
                                flagged[i][j] = true;
                            }
                        }
                        else if(flagged[i][j]) {
                            if(board->elements[i][j] == BOMB) {
                                flagged[i][j] = false;
                            }
                            else {
                                flagged[i][j] = false;
                            }
                        }
                    }
                }
            }
        }
        
        // Draws the visual representation of the board 
        // Checks for bombs/powerups/powerdowns/flags
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for(int i = 0;i<BOARDSIZE;i++)
        {
            for(int j = 0;j<BOARDSIZE;j++)
            {
                if(collided[i][j] == false) // Draws all the gray rectangles (Fog of War) that can be clicked on
                {
                    DrawRectangleRec(tiles[i][j], GRAY);
                    
                    if (flagged[i][j]) {
                        DrawTexture(textures[10],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                    }
                }
                else if(collided[i][j] == true) // Draws the peices that exist beneath the (Fog of War)
                {
                    if(board->elements[i][j] == BOMB){ // If any bomb is shown, the game instanstly loses
                        DrawTexture(textures[9],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                        status = LOST;
                    }
                    else if(board->elements[i][j] == POWERUP) // If powerup is shown, apply it and make that square EMPTY 
                    {
                        bool addedFlag = ApplyPowerUp(board, flagged);
                        if(addedFlag)
                        {
                            textBot.m = "A flag was placed!";
                        }
                        else{
                            textBot.m = "A bomb was removed!";
                        }
                        textTop.m = "PowerUp Applied";
                        board->elements[i][j] = EMPTY;
                        FillBoard(board);
                    }
                    else if(board->elements[i][j] == POWERDOWN) // If powerdown is shown, apply it and make that square EMPTY 
                    {
                        bool addedBomb = ApplyPowerDown(board, collided);
                        if(addedBomb)
                        {
                            textBot.m = "A bomb was added!";
                        }
                        else{
                            textBot.m = "Fog of war reset!";
                        }
                        textTop.m = "PowerDown Applied";
                        board->elements[i][j] = EMPTY;
                        FillBoard(board);
                    }
                    else{
                        DrawTexture(textures[board->elements[i][j]],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                    }
                }
            }
        }
        
        flagOnBombCount = 0;
        flagCount = 0;
        
        for(int i = 0;i<BOARDSIZE;i++)
        {
            for(int j = 0;j<BOARDSIZE;j++)
            {
                if(flagged[i][j] == true)
                {
                    if(board->elements[i][j] == BOMB)
                    {
                        flagOnBombCount++;
                    }
                    flagCount++;
                }
            }
        }
        
        // Changes status to WON if there is a flag on each
        // bomb and only if there is a flag on each bomb
        if(flagOnBombCount == BOMBAMOUNT && flagCount == BOMBAMOUNT)
        {
            status = WON;
        }
        
        EndDrawing();
    }
    CloseWindow();
    
    // Unloads and frees memory for items that were loaded or malloc'ed
    for(int i = 0;i<16;i++)
    {
        UnloadTexture(textures[i]);
    }
    UnloadFont(ourFont);
    free(textures);
    free(board);
    
    return 0;
}