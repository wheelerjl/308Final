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

int main()
{
    int screenWidth = 800;
    int screenHeight = 450;
    
    int boardHeight = 9;
    int boardWidth = 9;
    
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    
    Rectangle tiles[boardWidth][boardHeight];
    bool collided[boardWidth][boardHeight];
    
    //Texture2D bomb = LoadTexture("images/bomb.png");
    
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
            xStart+=15;
        }
        xStart = 15;
        yStart+=15;
    }
    
    
    for(int i = 0;i < boardWidth;i++)
    {
        for(int j = 0;j < boardHeight;j++)
        {
            collided[i][j] = false;
        }
    }
    
    Vector2 mousePoint;
    
    SetTargetFPS(10);
    
    while (!WindowShouldClose())
    {
        
        mousePoint = GetMousePosition();
        
        for(int i = 0;i < boardWidth;i++)
        {
            for(int j = 0;j < boardHeight;j++)
            {
                if(CheckCollisionPointRec(mousePoint, tiles[i][j]))
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
                    DrawTexture(textures[0],tiles[i][j].x + 1,tiles[i][j].y + 1, WHITE);
                }
            }
        }
        
        EndDrawing();
    }
    CloseWindow();
    
    return 0;
}