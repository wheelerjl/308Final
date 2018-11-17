/*******************************************************************************************
*
*   raylib [core] example - basic window
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "types.h"
#include <stdio.h>

int main()
{
    int screenWidth = 800;
    int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    
    /*
    Texture2D one = LoadTexture("images/one.png");
    Texture2D two = LoadTexture("images/two.png");
    Texture2D three = LoadTexture("images/three.png");
    Texture2D four = LoadTexture("images/four.png");
    Texture2D five = LoadTexture("images/five.png");
    Texture2D six = LoadTexture("images/six.png");
    Texture2D seven = LoadTexture("images/seven.png");
    Texture2D eight = LoadTexture("images/eight.png");
    Texture2D empty = LoadTexture("images/empty.png");
    Texture2D bomb = LoadTexture("images/bomb.png");
    Texture2D flag = LoadTexture("images/flag.png");
    
    Board hiddenBoard;
    Board shownBoard;
    */
    
    Texture2D bomb = LoadTexture("images/bomb.png");
    
    Rectangle recArray[16][16];
    bool collided[16][16];
    
    int xStart = 15;
    int yStart = 40;
    
    for(int i = 0;i < 16;i++)
    {
        for(int j = 0;j < 16;j++)
        {
            recArray[i][j].x = xStart;
            recArray[i][j].y = yStart;
            recArray[i][j].width = 15;
            recArray[i][j].height = 15;
            xStart+=15;
        }
        xStart = 15;
        yStart+=15;
    }
    
    
    for(int i = 0;i < 16;i++)
    {
        for(int j = 0;j < 16;j++)
        {
            collided[i][j] = false;
        }
    }
    
    Vector2 mousePoint;
    
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        mousePoint = GetMousePosition();
        
        for(int i = 0;i<16;i++)
        {
            for(int j = 0;j<16;j++)
            {
                if(CheckCollisionPointRec(mousePoint, recArray[i][j]))
                {
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        collided[i][j] = true;
                    }
                }
            }
        }
        
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        for(int i = 0;i<16;i++)
        {
            for(int j = 0;j<16;j++)
            {
                if(collided[i][j] == false)
                {
                    DrawRectangleRec(recArray[i][j], GRAY);
                }
                else
                {
                    DrawRectangleRec(recArray[i][j], WHITE);
                }
            }
        }
        
        for(int i = 0;i<16;i++)
        {
            for(int j = 0;j<16;j++)
            {
                if(collided[i][j] == true)
                {
                    DrawTexture(bomb,recArray[i][j].x + 1,recArray[i][j].y + 1, WHITE);
                }
            }
        }
        
        EndDrawing();
    } 
    
    CloseWindow();
    
    return 0;
}