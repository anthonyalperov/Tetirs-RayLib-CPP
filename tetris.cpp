#include "tetris.h"
#include <string>

// Unified cell size (locked to grid texture)
const int TETROMINOES[7][4][4] =
{
    // 1: T 
    {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    // 2: Z
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    // 3: S
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    // 4: L
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    // 5: J
    {
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    // 6: O
    {
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },

    // 7: I
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    }
};

void Tetris::AnimateLineClear(int row)
{
    for (int i = 0; i < 4; i++)
    {
        BeginDrawing();
        DrawBackground();
        DrawPlayfield();
        DrawLockedBlocks();

        Color flash = (i % 2 == 0) ? WHITE : BLACK;

        for (int x = 0; x < GRID_WIDTH; x++)
        {
            int px = gridOriginX + x * CELL;
            int py = gridOriginY + row * CELL;
            DrawRectangle(px, py, CELL, CELL, flash);
        }

        EndDrawing();
        WaitTime(0.05f);
    }
}


void Tetris::InitResources(const std::string& basePath)
{
    background = LoadTexture((basePath + "background.png").c_str());
	texT = LoadTexture((basePath + "blue_block.png").c_str()); //T "T_block.png"
	texZ = LoadTexture((basePath + "cyan_block.png").c_str()); //Z "Z_block.png"
	texS = LoadTexture((basePath + "yellow_block.png").c_str()); //S "S_block.png"
	texL = LoadTexture((basePath + "green_block.png").c_str()); //L "L_block.png"
	texJ = LoadTexture((basePath + "pink_block.png").c_str()); //J "J_block.png"
	texO = LoadTexture((basePath + "red_block.png").c_str()); //O "O_block.png"
    texI = LoadTexture((basePath + "purple_block.png").c_str()); //I "I_block.png"
    borderTexture = LoadTexture((basePath + "border.png").c_str());
    gridTexture = LoadTexture((basePath + "grid.png").c_str());
    holdLabel = LoadTexture((basePath + "hold.png").c_str());
    nextLabel = LoadTexture((basePath + "next.png").c_str());

    InitAudioDevice();
    sMove = LoadSound((basePath + "move.ogg").c_str());
    sRotate = LoadSound((basePath + "rotate.ogg").c_str());
    sDrop = LoadSound((basePath + "drop.ogg").c_str());
    sClear = LoadSound((basePath + "clear.ogg").c_str());
    sLevelUp = LoadSound((basePath + "levelup.ogg").c_str());
    sGameOver = LoadSound((basePath + "gameover.ogg").c_str());


    // Lock CELL to grid tile size (32×32)
    cellSize = (float)gridTexture.width;

    CreateIntro();
    CreateCredits();
    CreateControls();


    positionX = 300.0f;
    positionY = 300.0f;
}

void Tetris::UnloadResources()
{
    UnloadTexture(background);
    UnloadTexture(introTexture);
    UnloadTexture(creditsTexture);
    UnloadTexture(borderTexture);
    UnloadTexture(texT);
    UnloadTexture(texZ);
    UnloadTexture(texS);
    UnloadTexture(texL);
    UnloadTexture(texJ);
    UnloadTexture(texO);
    UnloadTexture(texI);
    UnloadTexture(gridTexture);
    UnloadTexture(holdLabel);
    UnloadTexture(nextLabel);

    UnloadSound(sMove);
    UnloadSound(sRotate);
    UnloadSound(sDrop);
    UnloadSound(sClear);
    UnloadSound(sLevelUp);
    UnloadSound(sGameOver);
    CloseAudioDevice();
}

void Tetris::loadBorder()
{
    Rectangle src = { 0, 0, (float)borderTexture.width, (float)borderTexture.height };
    Rectangle dst = { 50, 50, (float)borderTexture.width, (float)borderTexture.height };
    DrawTexturePro(borderTexture, src, dst, { 0, 0 }, 0.0f, WHITE);
}

void Tetris::DrawPlayfield()
{
    float bs = CELL;
    DrawBorder();
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            float gx = gridOriginX + x * bs;
            float gy = gridOriginY + y * bs;

            DrawTexture(gridTexture, (int)gx, (int)gy, WHITE);
        }
    }
}

void Tetris::DrawFallingPiece(const Logic& logic)
{
    float bs = CELL;

    Texture2D* tex = nullptr;
    switch (logic.currentType)
    {
    case 1: tex = &texT; break;
    case 2: tex = &texZ; break;
    case 3: tex = &texS; break;
    case 4: tex = &texL; break;
    case 5: tex = &texJ; break;
    case 6: tex = &texO; break;
    case 7: tex = &texI; break;
    }

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (logic.currentShape[y][x] == 1)
            {
                float dx = gridOriginX + (logic.pieceX + x) * bs;
                float dy = gridOriginY + (logic.pieceY + y) * bs;

                Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
                Rectangle dst = { dx, dy, bs, bs };

                DrawTexturePro(*tex, src, dst, { 0,0 }, 0.0f, WHITE);
            }
        }
    }
}


void Tetris::DrawBorder()
{
    float frame = 16.0f;

    float borderW = borderTexture.width;   // 1900
    float borderH = borderTexture.height;  // 2850

    float gridW = 10 * CELL;   // 320
    float gridH = 20 * CELL;   // 640

    // Scale border proportionally to fit height
    float scale = gridH / borderH;

    float drawW = borderW * scale;
    float drawH = borderH * scale;

    Rectangle src = { 0, 0, borderW, borderH };
    Rectangle dst = {
        gridOriginX - (drawW - gridW) / 2,
        gridOriginY - (drawH - gridH) / 2,
        drawW,
        drawH
    };

    DrawTexturePro(borderTexture, src, dst, { 0,0 }, 0.0f, WHITE);
}

void Tetris::DrawLockedBlocks()
{
    float bs = CELL;

    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            int cell = board[y][x];
            if (cell == 0) continue;

            Texture2D* tex = nullptr;
            switch (cell)
            {
            case 1: tex = &texT; break;
            case 2: tex = &texZ; break;
            case 3: tex = &texS; break;
            case 4: tex = &texL; break;
            case 5: tex = &texJ; break;
            case 6: tex = &texO; break;
            case 7: tex = &texI; break;
            }

            float dx = gridOriginX + x * bs;
            float dy = gridOriginY + y * bs;

            Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
            Rectangle dst = { dx, dy, bs, bs };

            DrawTexturePro(*tex, src, dst, { 0,0 }, 0.0f, WHITE);
        }
    }
}

void Tetris::DrawGhostPiece(const Logic& logic)
{
    int ghostX = logic.pieceX;
    int ghostY = logic.pieceY;

    // Copy board
    int (*board)[10] = this->board;

    // Drop ghost until collision
    while (true)
    {
        bool collision = false;

        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (logic.currentShape[y][x] == 1)
                {
                    int bx = ghostX + x;
                    int by = ghostY + y;

                    if (by + 1 >= 20 || board[by + 1][bx] != 0)
                    {
                        collision = true;
                        break;
                    }
                }
            }
            if (collision) break;
        }

        if (collision)
            break;

        ghostY++;
    }

    // Draw ghost (semi-transparent)
    Color ghostColor = { 200, 200, 200, 80 };

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (logic.currentShape[y][x] == 1)
            {
                int px = gridOriginX + (ghostX + x) * CELL;
                int py = gridOriginY + (ghostY + y) * CELL;

                DrawRectangle(px, py, CELL, CELL, ghostColor);
            }
        }
    }
}

void Tetris::DrawNextPiece(const Logic& logic)
{
    int offsetX = gridOriginX + 9 * CELL + 80;
    int offsetY = gridOriginY + 120;

    int next = logic.nextType;
    Texture2D tex = GetTextureForType(next);

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (TETROMINOES[next - 1][y][x] == 1)
            {
                int px = offsetX + x * CELL;
                int py = offsetY + y * CELL;

                DrawTexture(tex, px, py, WHITE);
            }
        }
    }
}




void Tetris::DrawBlock(int x, int y, int type)
{
    // type is 1–7, matching your tetromino textures
    DrawTexture(blockTextures[type - 1], x, y, WHITE);
}

Texture2D Tetris::GetTextureForType(int type)
{
    switch (type)
    {
    case 1: return texT;
    case 2: return texZ;
    case 3: return texS;
    case 4: return texL;
    case 5: return texJ;
    case 6: return texO;
    case 7: return texI;
    }
    return texO;
}


void Tetris::DrawHoldPiece(const Logic& logic)
{
    if (logic.holdType == 0)
        return; // nothing to draw yet

    int offsetX = gridOriginX - 160;  // left side
    int offsetY = gridOriginY + 120;

    int type = logic.holdType;
    Texture2D tex = GetTextureForType(type);

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (TETROMINOES[type - 1][y][x] == 1)
            {
                int px = offsetX + x * CELL;
                int py = offsetY + y * CELL;

                DrawTexture(tex, px, py, WHITE);
            }
        }
    }
}

void Tetris::DrawScore(const Logic& logic)
{
    DrawText(TextFormat("Score: %d", logic.score),
        gridOriginX + 10 * CELL + 50,
        gridOriginY + 350,
        40,
        WHITE);

    DrawText(TextFormat("Lines: %d", logic.linesClearedTotal),
        gridOriginX + 10 * CELL + 50,
        gridOriginY + 400,
        40,
        WHITE);
}

void Tetris::DrawHighScore(const Logic& logic)
{
    DrawText(TextFormat("High Score: %d", logic.highScore),
        gridOriginX + 10 * CELL + 50,
        gridOriginY + 500,
        40,
        WHITE);
}








void Tetris::loadAllImages()
{
    DrawBackground();
    DrawTexture(introTexture, 100, 50, WHITE);
    DrawTexture(creditsTexture, 100, 150, WHITE);
    DrawPlayfield();
}

void Tetris::ClearBoard()
{
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            board[y][x] = 0;
        }
    }
}

void Tetris::CreateIntro()
{
    int fontSize = 40;
    const char* msg = "Welcome to Tetris! (With No Ads) Enjoy!";
    int textW = MeasureText(msg, fontSize);
    int textH = fontSize;

    int paddingLeft = 650;
    int paddingTop = 0;

    Image img = GenImageColor(textW + paddingLeft, textH + paddingTop, BLANK);
    ImageDrawText(&img, msg, paddingLeft, paddingTop, fontSize, WHITE);
    introTexture = LoadTextureFromImage(img);
    UnloadImage(img);
}

void Tetris::CreateCredits()
{
    int fontSize = 40;
    const char* msg = "Made by Anthony Alperov";
    int textW = MeasureText(msg, fontSize);
    int textH = fontSize;

    int paddingLeft = 650;
    int paddingTop = 25;

    Image img = GenImageColor(textW + paddingLeft, textH + paddingTop, BLANK);
    ImageDrawText(&img, msg, paddingLeft, paddingTop, fontSize, WHITE);
    creditsTexture = LoadTextureFromImage(img);
    UnloadImage(img);
}

void Tetris::CreateControls()
{
    int fontSize = 40;

    const char* lines[] = {
        "CONTROLS",
        "",
        "Move Left:      LEFT ARROW",
        "Move Right:     RIGHT ARROW",
        "Soft Drop:      DOWN ARROW",
        "Hard Drop:      SPACE",
        "Rotate:         UP ARROW",
        "Hold Piece:     C",
        "Restart:        R",
        "Pause:          P (optional)",
        "",
        "Press H to close"
    };

    int lineCount = sizeof(lines) / sizeof(lines[0]);

    int width = 800;
    int height = lineCount * (fontSize + 10) + 40;

    Image img = GenImageColor(width, height, BLANK);

    int y = 20;
    for (int i = 0; i < lineCount; i++)
    {
        ImageDrawText(&img, lines[i], 40, y, fontSize, WHITE);
        y += fontSize + 10;
    }

    controlsTexture = LoadTextureFromImage(img);
    UnloadImage(img);
}


void Tetris::DrawBackground()
{
    DrawTexturePro(
        background,
        { 0, 0, (float)background.width, (float)background.height },
        { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        { 0, 0 },
        0.0f,
        WHITE
    );
}

int Tetris::updateScore(int linesCleared)
{
    score += linesCleared * 100;
    return score;
}
