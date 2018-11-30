//
//  main.c
//
//  Created by Jordan Wheeler
//

#include "raylib.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>

int BOARDSIZE = 9;
int BOMBAMOUNT = 10;
int POWERAMOUNT = 2;

void FillTextureArray(int count, Texture2D * text) {
    const char * strings[] = {"images/empty.png", "images/one.png", "images/two.png", "images/three.png", 
                            "images/four.png", "images/five.png", "images/six.png", "images/seven.png", 
                            "images/eight.png", "images/bomb.png","images/flag.png","images/PowerDown.png",
                            "images/PowerUp.png","images/easy.png", "images/medium.png", "images/hard.png" };
    for(int i = 0; i < count;i++)
    {
        text[i] = LoadTexture(strings[i]);
    }
}

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

/*void ApplyPowerUp(bool **col, Board * board, Board * flags) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(board->elements[i][j] == BOMB && flags->elements[i][j] == EMPTY) {
                col[i][j] = true;
                flags->elements[i][j] = FLAG;
            }
        }
    }
}*/

/*void ApplyPowerDown(bool **col, Board * board, Board * flags) {
    int x = GetRandomValue(0, 8);
    int y = GetRandomValue(0, 8);
    
    //if() {
        //b->elements[x][y] = BOMB;
        //bombCount--;
    //}
}*/

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
            if((b->elements[i - 1][j + 1] != BOMB) && (b->elements[i - 1][j + 1] != POWERUP) && (b->elements[i - 1][j] != POWERDOWN)) {
                col[i - 1][j + 1] = true;
            }
        }
        //Checking bottom middle
        if((b->elements[i][j + 1] != BOMB) && (b->elements[i][j + 1] != POWERUP) && (b->elements[i][j] != POWERDOWN)) {
            col[i][j + 1] = true;
        }
        //Checking bottom right 
        if(i < 8) {
            if((b->elements[i + 1][j + 1] != BOMB) && (b->elements[i + 1][j + 1] != POWERUP) && (b->elements[i + 1][j] != POWERDOWN)) {
                col[i + 1][j + 1] = true;
            }
        }
    }
}

void AddTiles(Rectangle tiles[BOARDSIZE][BOARDSIZE]) {
    
    int xStart = 15;
    int yStart = 40;
    
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

int GetInitialSize() {
    int screenWidth = 153;
    int screenHeight = 40;
    
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
    
    int textureCount = 16;
    Texture2D * textures = malloc(textureCount*sizeof(Texture2D));
    FillTextureArray(textureCount, textures);
    
    SetTargetFPS(30);
    
    bool choseSize = false;
    
    while (!WindowShouldClose())
    {
        mouseLocation = GetMousePosition();
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        for(int i = 0;i<3;i++)
        {
            DrawRectangleRec(buttons[i],WHITE);
            DrawTexture(textures[13+i],buttons[i].x, buttons[i].y,WHITE);
        }
        
        if(CheckCollisionPointRec(mouseLocation, buttons[0]))
        {
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                BOARDSIZE = 9;
                BOMBAMOUNT = 10;
                POWERAMOUNT = 2;
            }
        }
        else if(CheckCollisionPointRec(mouseLocation, buttons[1]))
        {
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                BOARDSIZE = 16;
                BOMBAMOUNT = 40;
                POWERAMOUNT = 8;
            }
        }
        else if(CheckCollisionPointRec(mouseLocation, buttons[2]))
        {
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                BOARDSIZE = 24;
                BOMBAMOUNT = 90;
                POWERAMOUNT = 12;
            }
        }
        
        EndDrawing();
    } 
    CloseWindow();
    
    free(textures);
}

int main() {
    
    // Asks the user what difficulty to use
    GetInitialSize();
    
    int screenWidth = (16*BOARDSIZE)+30;
    int screenHeight = (16*BOARDSIZE)+100;
    
    InitWindow(screenWidth, screenHeight, "Spiked Ball Sweeper");
    
    int textureCount = 16;
    Texture2D * textures = malloc(textureCount*sizeof(Texture2D));
    FillTextureArray(textureCount, textures);
    
    // Fills the array holding the Rectangles required for collision detection.
    // Each element in tiles represents a square on the board.
    Rectangle tiles[BOARDSIZE][BOARDSIZE];
    
    AddTiles(tiles);
    
    bool collided[BOARDSIZE][BOARDSIZE];                            // Initializing collided
    bool flagged[BOARDSIZE][BOARDSIZE];                             // Initializing flagged
	Board * board = malloc(sizeof(piece[BOARDSIZE][BOARDSIZE]));    // Initializing board
    
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
    
    SetTargetFPS(30);
    
    bool firstClick = true;
    bool bombClicked = false;
    
    int flagOnBombCount = 0;
    int flagCount = 0;
    
    // Renews the display continuously until the program is closed or Esc key is pressed.
    while (!WindowShouldClose())
    {
        mouseLocation = GetMousePosition();
        
        // Determines Win Condition
        if(flagOnBombCount == BOMBAMOUNT && flagCount == BOMBAMOUNT)
        {
            for(int i = 0;i<BOARDSIZE;i++)
            {
                for(int j = 0;j<BOARDSIZE;j++)
                {
                    collided[i][j] = false;
                }
            }
            DrawText("You Won!. \nYou got gud.", 15, screenHeight-50, 12, BLACK);
        }
        // Deterines Loss Condition
        else if(bombClicked)
        {
            for(int i = 0;i<BOARDSIZE;i++)
            {
                for(int j = 0;j<BOARDSIZE;j++)
                {
                    collided[i][j] = true;
                }
            }
            DrawText("You hit a bomb, you lose. \nGit gud.", 15, screenHeight-50, 12, BLACK);
        }
        
        for(int i = 0;i < BOARDSIZE;i++)
        {
            for(int j = 0;j < BOARDSIZE;j++)
            {
                if(CheckCollisionPointRec(mouseLocation, tiles[i][j]))
                {
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && bombClicked == false && flagged[i][j] == false)
                    {
                        if(firstClick) {
                            RandomizeBoard(board, i, j);
                            firstClick = false;
                        }/*
                        if(collided[i][j] == false) {
                            if(board->elements[i][j] == POWERUP) {
                                ApplyPowerUp();
                            }
                            else if(board->elements[i][j] == POWERDOWN) {
                                ApplyPowerDown();
                            }
                         }*/
                        collided[i][j] = true;
                        CheckSurroundingSpaces(i, j, collided, board);
                    }
                    else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && bombClicked == false)
                    {
                        if(!flagged[i][j]) 
                        {
                            if(board->elements[i][j] == BOMB)
                            {
                                flagged[i][j] = true;
                                flagCount++;
                                flagOnBombCount++;
                            }
                            else
                            {
                                flagged[i][j] = true;
                                flagCount++;
                            }
                        }
                        else if(flagged[i][j])
                        {
                            if(board->elements[i][j] == BOMB)
                            {
                                flagged[i][j] = false;
                                flagCount--;
                                flagOnBombCount--;
                            }
                            else
                            {
                                flagged[i][j] = false;
                                flagCount--;
                            }
                        }
                    }
                    
                    // Allows you to press spacebar to reveal the board.
                    else if(IsKeyPressed(KEY_SPACE))
                    {
                        for(int c = 0;c<BOARDSIZE;c++)
                        {
                            for(int d = 0;d<BOARDSIZE;d++)
                            {
                                collided[c][d] = true;
                            }
                        }
                    }
                }
            }
        }
		
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        for(int i = 0;i<BOARDSIZE;i++)
        {
            for(int j = 0;j<BOARDSIZE;j++)
            {
                if(collided[i][j] == false)
                {
                    DrawRectangleRec(tiles[i][j], GRAY);
                    
                    if (flagged[i][j]) {
                        DrawTexture(textures[10],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                    }
                }
                else if(collided[i][j] == true)
                {
                    if(board->elements[i][j] == BOMB && flagged[i][j] == false){
                        DrawTexture(textures[9],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                        bombClicked = true;
                    }
                    else{
                        DrawTexture(textures[board->elements[i][j]],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                    }
                }
            }
        }
        
        EndDrawing();
    }
    CloseWindow();
    
    free(textures);
    free(board);
    
    return 0;
}