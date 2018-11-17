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
                        collided[i][j] = true;
                    }
                    else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
                    {
                        collided[i][j] = false;
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