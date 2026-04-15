#include "raylib.h"
#include "tetris.h"
#include <string>

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Tetris");

    std::string sprite_setup = "C:\\Users\\malpe\\OneDrive\\Desktop\\Project\\raylib\\raylib\\resources\\";


    Tetris tetris;
    tetris.InitResources(sprite_setup);

    SetTargetFPS(15);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        tetris.loadAllImages();

        EndDrawing();
    }

    tetris.UnloadResources();
    CloseWindow();

    return 0;
}
