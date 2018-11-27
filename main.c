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
                            "images/eight.png", "images/bomb.png","images/flag.png"};
    for(int i = 0; i < count;i++)
    {
        text[i] = LoadTexture(strings[i]);
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
    
    int textureCount = 11;
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
    
    //Fill board with empty spaces
    for(int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board->elements[i][j] = EMPTY;
        }
    }
    
    bool firstClick = true;
    
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
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        if(firstClick) {
                            RandomizeBoard(board, i, j);
                            firstClick = false;
                        }
                        collided[i][j] = true;
                    }
                    else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
                    {
                        collided[i][j] = false;
                        //Test codes
                        for(int a = 0; a < 9; a++) {
                            for(int b = 0; b < 9; b++) {
                                collided[a][b] = true;
                            }
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
                    if(board->elements[i][j] == BOMB){
                        DrawTexture(textures[9],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                    }
                    else if(board->elements[i][j] == POWERUP) {
                        DrawTexture(textures[1],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                    }
                    else if(board->elements[i][j] == POWERDOWN) {
                        DrawTexture(textures[2],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                    }
                    else {
                        DrawTexture(textures[0],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                    }
                }
            }
        }
        
        EndDrawing();
    }
    CloseWindow();
    
    return 0;
}