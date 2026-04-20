#include "raylib.h"
#include "tetris.h"
#include "logic.h"
#include <string>
#include <ctime>

enum GameState
{
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAMEOVER
};

int main()
{
    srand((unsigned int)time(NULL));

    const int screenWidth = 1920;
    const int screenHeight = 1200;

    InitWindow(screenWidth, screenHeight, "Tetris");
    SetWindowState(FLAG_FULLSCREEN_MODE);

    std::string sprite_setup =
        "C:\\Users\\malpe\\OneDrive\\Desktop\\Project\\TetrisC++\\TetrisC++\\resources\\";

    Tetris tetris;

    tetris.gridOriginX = GetScreenWidth() / 2 - (10 * CELL) / 2;
    tetris.gridOriginY = GetScreenHeight() / 2 - (20 * CELL) / 2 - 50;

    Logic logic;
    logic.LoadHighScore();

    tetris.InitResources(sprite_setup);

    SetTargetFPS(60);

    float fallTimer = 0.0f;
    float baseFallDelay = 0.5f;

    float softDropTimer = 0.0f;
    float softDropDelay = 0.05f;

    bool showControls = false;
    bool paused = false;

    GameState state = STATE_MENU;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        fallTimer += dt;
        softDropTimer += dt;

        // ---------------------------------------------------------
        // STATE: MENU
        // ---------------------------------------------------------
        if (state == STATE_MENU)
        {
            if (IsKeyPressed(KEY_H))
                showControls = !showControls;

            if (!showControls && IsKeyPressed(KEY_ENTER))
            {
                tetris.ClearBoard();
                logic.score = 0;
                logic.linesClearedTotal = 0;
                logic.level = 1;
                logic.linesToNextLevel = 10;

                if (logic.SpawnPiece(tetris.getBoard()))
                    state = STATE_GAMEOVER;
                else
                    state = STATE_PLAYING;
            }

            BeginDrawing();
            tetris.DrawBackground();

            if (showControls)
            {
                DrawTexture(tetris.controlsTexture,
                    GetScreenWidth() / 2 - tetris.controlsTexture.width / 2,
                    GetScreenHeight() / 2 - tetris.controlsTexture.height / 2,
                    WHITE);

                DrawText("Press H to return",
                    GetScreenWidth() / 2 - 150,
                    GetScreenHeight() / 2 + tetris.controlsTexture.height / 2 + 20,
                    30,
                    YELLOW);

                EndDrawing();
                continue;
            }

            DrawText("TETRIS --> Tony's Edition",
                GetScreenWidth() / 2 - 300,
                GetScreenHeight() / 2 - 200,
                60,
                WHITE);

            DrawText("PRESS ENTER TO START",
                GetScreenWidth() / 2 - 260,
                GetScreenHeight() / 2 - 50,
                40,
                YELLOW);

            DrawText("PRESS H FOR CONTROLS",
                GetScreenWidth() / 2 - 260,
                GetScreenHeight() / 2 + 10,
                30,
                LIGHTGRAY);

            DrawText("PRESS ESC TO QUIT",
                GetScreenWidth() / 2 - 220,
                GetScreenHeight() / 2 + 60,
                30,
                GRAY);

            EndDrawing();
            continue;
        }

        // ---------------------------------------------------------
        // STATE: GAMEOVER
        // ---------------------------------------------------------
        if (state == STATE_GAMEOVER)
        {
            BeginDrawing();
            ClearBackground(BLACK);

            DrawText("GAME OVER",
                GetScreenWidth() / 2 - 250,
                GetScreenHeight() / 2 - 80,
                100,
                RED);

            DrawText("Press R to Restart",
                GetScreenWidth() / 2 - 200,
                GetScreenHeight() / 2 + 50,
                40,
                WHITE);

            DrawText("Press M for Main Menu",
                GetScreenWidth() / 2 - 230,
                GetScreenHeight() / 2 + 100,
                30,
                GRAY);

            EndDrawing();

            if (logic.score > logic.highScore)
            {
                logic.highScore = logic.score;
                logic.SaveHighScore();
            }

            if (IsKeyPressed(KEY_R))
            {
                tetris.ClearBoard();
                logic.score = 0;
                logic.linesClearedTotal = 0;
                logic.level = 1;
                logic.linesToNextLevel = 10;

                if (logic.SpawnPiece(tetris.getBoard()))
                    state = STATE_GAMEOVER;
                else
                    state = STATE_PLAYING;
            }

            if (IsKeyPressed(KEY_M))
            {
                state = STATE_MENU;
            }

            continue;
        }

        // ---------------------------------------------------------
        // STATE: PLAYING
        // ---------------------------------------------------------

        if (IsKeyPressed(KEY_P))
            paused = !paused;

        if (paused)
        {
            BeginDrawing();
            tetris.DrawBackground();
            tetris.DrawPlayfield();
            tetris.DrawLockedBlocks();
            tetris.DrawGhostPiece(logic);
            tetris.DrawFallingPiece(logic);
            tetris.DrawNextPiece(logic);
            tetris.DrawHoldPiece(logic);
            tetris.DrawScore(logic);
            tetris.DrawHighScore(logic);

            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{ 0, 0, 0, 150 });

            DrawText("PAUSED",
                GetScreenWidth() / 2 - 150,
                GetScreenHeight() / 2 - 50,
                100,
                WHITE);

            EndDrawing();
            continue;
        }

        float effectiveFallDelay = baseFallDelay / (float)logic.level;
        if (effectiveFallDelay < 0.05f)
            effectiveFallDelay = 0.05f;

        if (fallTimer >= effectiveFallDelay)
        {
            fallTimer = 0.0f;

            if (!logic.CheckCollisionDown(tetris.getBoard()))
            {
                logic.pieceY++;
            }
            else
            {
                logic.LockPiece(tetris.getBoard());
                logic.holdUsed = false;

                int cleared = logic.ClearLines(tetris.getBoard());

                if (cleared > 0)
                {
                    PlaySound(tetris.sClear);

                    int (*board)[10] = tetris.getBoard();
                    for (int y = 0; y < 20; y++)
                    {
                        bool full = true;
                        for (int x = 0; x < 10; x++)
                            if (board[y][x] == 0)
                                full = false;

                        if (full)
                            tetris.AnimateLineClear(y);
                    }
                }

                if (logic.SpawnPiece(tetris.getBoard()))
                {
                    PlaySound(tetris.sGameOver);
                    state = STATE_GAMEOVER;
                }
            }
        }

        // INPUT
        if (IsKeyPressed(KEY_UP))
        {
            logic.Rotate(tetris.getBoard());
            PlaySound(tetris.sRotate);
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            logic.HardDrop(tetris.getBoard());
            PlaySound(tetris.sDrop);

            int cleared = logic.ClearLines(tetris.getBoard());
            if (cleared > 0)
                PlaySound(tetris.sClear);

            if (logic.SpawnPiece(tetris.getBoard()))
            {
                PlaySound(tetris.sGameOver);
                state = STATE_GAMEOVER;
            }
        }

        if (IsKeyDown(KEY_DOWN) && softDropTimer >= softDropDelay)
        {
            softDropTimer = 0.0f;
            logic.MoveDown(tetris.getBoard());
        }

        if (IsKeyPressed(KEY_LEFT))
        {
            logic.MoveLeft(tetris.getBoard());
            PlaySound(tetris.sMove);
        }

        if (IsKeyPressed(KEY_RIGHT))
        {
            logic.MoveRight(tetris.getBoard());
            PlaySound(tetris.sMove);
        }

        if (IsKeyPressed(KEY_C))
        {
            logic.HoldPiece(tetris.getBoard());
        }

        if (IsKeyPressed(KEY_H))
            showControls = !showControls;

        // DRAW
        BeginDrawing();
        tetris.DrawBackground();
        DrawTexture(tetris.introTexture, 100, 50, WHITE);
        DrawTexture(tetris.creditsTexture, 100, 150, WHITE);

        DrawTexture(tetris.holdLabel, tetris.gridOriginX - 200, tetris.gridOriginY + 50, WHITE);
        DrawTexture(tetris.nextLabel, tetris.gridOriginX + 8 * CELL + 50, tetris.gridOriginY + 50, WHITE);

        tetris.DrawScore(logic);
        tetris.DrawHighScore(logic);
        tetris.DrawPlayfield();
        tetris.DrawLockedBlocks();
        tetris.DrawGhostPiece(logic);
        tetris.DrawFallingPiece(logic);
        tetris.DrawNextPiece(logic);
        tetris.DrawHoldPiece(logic);

        if (showControls)
        {
            DrawTexture(tetris.controlsTexture,
                GetScreenWidth() / 2 - tetris.controlsTexture.width / 2,
                GetScreenHeight() / 2 - tetris.controlsTexture.height / 2,
                WHITE);

            DrawText("Press H to return",
                GetScreenWidth() / 2 - 150,
                GetScreenHeight() / 2 + tetris.controlsTexture.height / 2 + 20,
                30,
                YELLOW);

            EndDrawing();
            continue;
        }

        EndDrawing();
    }

    tetris.UnloadResources();
    CloseWindow();

    return 0;
}
