//
//  main.c
//
//  Created by Jordan Wheeler
//

#include "raylib.h"
#include "types.h"
#include <stdio.h>

void FillTextureArray(int count, Texture2D * text)
{
    const char * strings[] = {"images/empty.png", "images/one.png", "images/two.png", "images/three.png", 
                            "images/four.png", "images/five.png", "images/six.png", "images/seven.png", 
                            "images/eight.png", "images/bomb.png","images/flag.png","images/PowerDown.png","images/PowerUp.png"};
    for(int i = 0; i < count;i++)
    {
        text[i] = LoadTexture(strings[i]);
    }
}

void FillBoard(Board * b) {
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
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
                    if(i < 8) {
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
                if(i < 8) {
                    if(b->elements[i + 1][j] == BOMB) {
                        count++;
                    }
                }
                //Checking bottom 
                if(j < 8) {
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
                    if(i < 8) {
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
    int bombCount = 10;
    int upCount = 5;
    int downCount = 5;
    
    //Adds bombs to board 
    while(bombCount != 0) {
        int x = GetRandomValue(0, 8);
        int y = GetRandomValue(0, 8);
        
        if(!(x == i && y == j) && b->elements[x][y] == EMPTY) {
            b->elements[x][y] = BOMB;
            bombCount--;
        }
    }
    
    //Adds Power Ups to Board 
    while(upCount != 0) {
        int x = GetRandomValue(0, 8);
        int y = GetRandomValue(0, 8);
        
        if(!(x == i && y == j) && b->elements[x][y] == EMPTY) {
            b->elements[x][y] = POWERUP;
            upCount--;
        }
    }
    
    //Adds Power Downs to Board 
    while(downCount != 0) {
        int x = GetRandomValue(0, 8);
        int y = GetRandomValue(0, 8);
        
        if(!(x == i && y == j) && b->elements[x][y] == EMPTY) {
            b->elements[x][y] = POWERDOWN;
            downCount--;
        }
    }
    
    FillBoard(b);
}

void ApplyPowerUp(bool **col, Board * board, Board * flags) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(board->elements[i][j] == BOMB && flags->elements[i][j] == EMPTY) {
                col[i][j] = true;
                flags->elements[i][j] = FLAG;
            }
        }
    }
}

void ApplyPowerDown(bool **col, Board * board, Board * flags) {
    int x = GetRandomValue(0, 8);
    int y = GetRandomValue(0, 8);
    
    //if() {
        //b->elements[x][y] = BOMB;
        //bombCount--;
    //}
}

void CheckSurroundingSpaces(int i, int j, bool col[][9], Board * b) {
    
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

int main()
{
    int screenWidth = 174;
    int screenHeight = 199;
    
    int boardHeight = 9;
    int boardWidth = 9;
    
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    
    Rectangle tiles[boardWidth][boardHeight];
    bool collided[boardWidth][boardHeight];
    
    int textureCount = 13;
    Texture2D * textures = malloc(textureCount*sizeof(Texture2D));
    FillTextureArray(textureCount, textures);
    
    int xStart = 15;
    int yStart = 40;
    
    for(int i = 0;i < boardWidth;i++)
    {
        for(int j = 0;j < boardHeight;j++)
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
    
    for(int i = 0;i < boardWidth;i++)
    {
        for(int j = 0;j < boardHeight;j++)
        {
            collided[i][j] = false;
        }
    }
    
    Vector2 mouseLocation;
    
    SetTargetFPS(30);
	
    //Allocate memory for Board 
	Board * board = malloc(sizeof(piece[9][9]));
    
    Board * flags = malloc(sizeof(piece[9][9]));
    
    //Fill board with empty spaces
    for(int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board->elements[i][j] = EMPTY;
            board->elements[i][j] = EMPTY;
        }
    }
    
    bool firstClick = true;
    bool bombClicked = false;
    
    // Renews the display continuously until the program is closed or Esc key is pressed.
    while (!WindowShouldClose())
    {
        mouseLocation = GetMousePosition();
        
        for(int i = 0;i < boardWidth;i++)
        {
            for(int j = 0;j < boardHeight;j++)
            {
                if(CheckCollisionPointRec(mouseLocation, tiles[i][j]))
                {
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && bombClicked == false)
                    {
                        if(firstClick) {
                            RandomizeBoard(board, i, j);
                            firstClick = false;
                        }
                        collided[i][j] = true;
                        CheckSurroundingSpaces(i, j, collided, board);
                    }
                    else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && bombClicked == false)
                    {
                        if(flags->elements[i][j] == EMPTY) {
                            flags->elements[i][j] = FLAG;
                            collided[i][j] = true;
                        }
                        else {
                            flags->elements[i][j] = EMPTY;
                            collided[i][j] = false;
                        }
                    }
                }
            }
        }
		
		
        
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        for(int i = 0;i<boardWidth;i++)
        {
            for(int j = 0;j<boardHeight;j++)
            {
                if(collided[i][j] == false)
                {
                    DrawRectangleRec(tiles[i][j], GRAY);
                }
                else
                {
                    DrawRectangleRec(tiles[i][j], WHITE);
                }
            }
        }
        
        for(int i = 0;i<boardWidth;i++)
        {
            for(int j = 0;j<boardHeight;j++)
            {
                if(collided[i][j] == true)
                {
                    if(board->elements[i][j] == BOMB && flags->elements[i][j] != FLAG){
                        DrawTexture(textures[9],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                        DrawText("You hit a bomb, you lose. \nGit gud.", 0, 0, 12, BLACK);
                        bombClicked = true;
                    }
                    else if(board->elements[i][j] == POWERUP) {
                        DrawTexture(textures[12],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                    }
                    else if(board->elements[i][j] == POWERDOWN) {
                        DrawTexture(textures[11],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                    }
                    else{
                        DrawTexture(textures[board->elements[i][j]],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                    }
                    
                    if (flags->elements[i][j] == FLAG) {
                        DrawTexture(textures[10],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                    }
                }
            }
        }
        
        EndDrawing();
    }
    CloseWindow();
    
    return 0;
}