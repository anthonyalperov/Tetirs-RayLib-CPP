/*
I added 1.5 everywhere for now because I am trying to increase size of the whole gameplay scene.
*/

#include "tetris.h"
#include <string>

void Tetris::InitResources(const std::string& basePath)
{
    background = LoadTexture((basePath + "background.png").c_str());
    texT = LoadTexture((basePath + "blue_T1.png").c_str());
    texZ = LoadTexture((basePath + "cyan_Z1.png").c_str());
    texS = LoadTexture((basePath + "yellow_S1.png").c_str());
    texL = LoadTexture((basePath + "green_l1.png").c_str());
    texJ = LoadTexture((basePath + "pink_J1.png").c_str());
    texO = LoadTexture((basePath + "red_O1.png").c_str());
    texI = LoadTexture((basePath + "purple_I1.png").c_str());
    borderTexture = LoadTexture((basePath + "border.png").c_str());
    gridTexture = LoadTexture((basePath + "grid.png").c_str());

    CreateIntro();
    CreateCredits();

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
}

void Tetris::loadTblock()
{
    Rectangle src = { 0, 0, (float)texT.width, (float)texT.height };
    Rectangle dst = { 100.0f, 100.0f, BLOCK_SIZE_X * 1.5, BLOCK_SIZE_Y * 1.5 };
    DrawTexturePro(texT, src, dst, { 0,0 }, 0.0f, WHITE);
}

void Tetris::loadZblock()
{
    Rectangle src = { 0, 0, (float)texZ.width, (float)texZ.height };
    Rectangle dst = { 250.0f, 100.0f, BLOCK_SIZE_X * 1.5, BLOCK_SIZE_Y * 1.5 };
    DrawTexturePro(texZ, src, dst, { 0,0 }, 0.0f, WHITE);
}

void Tetris::loadSblock()
{
    Rectangle src = { 0, 0, (float)texS.width, (float)texS.height };
    Rectangle dst = { 400.0f, 100.0f, BLOCK_SIZE_X * 1.5, BLOCK_SIZE_Y * 1.5 };
    DrawTexturePro(texS, src, dst, { 0,0 }, 0.0f, WHITE);
}

void Tetris::loadLblock()
{
    Rectangle src = { 0, 0, (float)texL.width, (float)texL.height };
    Rectangle dst = { 100.0f, 250.0f, BLOCK_SIZE_X * 1.5, BLOCK_SIZE_Y * 1.5 };
    DrawTexturePro(texL, src, dst, { 0,0 }, 0.0f, WHITE);
}

void Tetris::loadJblock()
{
    Rectangle src = { 0, 0, (float)texJ.width, (float)texJ.height };
    Rectangle dst = { 250.0f, 250.0f, BLOCK_SIZE_X * 1.5, BLOCK_SIZE_Y * 1.5 };
    DrawTexturePro(texJ, src, dst, { 0,0 }, 0.0f, WHITE);
}

void Tetris::loadOblock()
{
    Rectangle src = { 0, 0, (float)texO.width, (float)texO.height };
    Rectangle dst = { 400.0f, 250.0f, BLOCK_SIZE_X * 1.5, BLOCK_SIZE_Y * 1.5 };
    DrawTexturePro(texO, src, dst, { 0,0 }, 0.0f, WHITE);
}

void Tetris::loadIblock()
{
    Rectangle src = { 0, 0, (float)texI.width, (float)texI.height };
    Rectangle dst = { 250.0f, 400.0f, (BLOCK_SIZE_X + 20) * 1.5, (BLOCK_SIZE_Y + 20) * 1.5 };
    DrawTexturePro(texI, src, dst, { 0,0 }, 0.0f, WHITE);
}

void Tetris::loadBorder()
{
    float borderW = (10 * MAXSIZE_X) * 1.5;
    float borderH = (20 * MAXSIZE_Y) * 1.5;

    Rectangle src = { 0, 0, (float)borderTexture.width, (float)borderTexture.height };
    Rectangle dst = { 50, 0, borderW, borderH };

    DrawTexturePro(borderTexture, src, dst, { 0,0 }, 0.0f, WHITE);
}

void Tetris::DrawPlayfield()
{
    float bs = BLOCK_SIZE * SCALE;   // 40 * 1.5 = 60

    //Draw Border First
    float borderW = GRID_WIDTH * bs;
    float borderH = GRID_HEIGHT * bs;

    Rectangle borderSrc = { 0, 0, (float)borderTexture.width, (float)borderTexture.height };
    Rectangle borderDst = { 50, 50, borderW, borderH };     // Same position as grid
    DrawTexturePro(borderTexture, borderSrc, borderDst, { 0,0 }, 0.0f, WHITE);

    // Draw Grid Inside the Border
    float innerOffset = 7.0f;   //Change this if needed, bigger = more inset

    Rectangle gridSrc = { 0, 0, (float)gridTexture.width, (float)gridTexture.height };
    Rectangle gridDst = {
        50 + innerOffset,
        50 + innerOffset,
        borderW - innerOffset * 2,
        borderH - innerOffset * 2
    };
    DrawTexturePro(gridTexture, gridSrc, gridDst, { 0,0 }, 0.0f, WHITE);

    //Draw blocks on top of the grid
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            if (board[y][x] != 0)
            {
                Rectangle blockDst = {
                    50 + innerOffset + x * bs,
                    50 + innerOffset + y * bs,
                    bs, bs
                };

                Color color = BLUE;
                if (board[y][x] == 1) color = RED;
                if (board[y][x] == 2) color = GREEN;
                if (board[y][x] == 3) color = YELLOW;
                if (board[y][x] == 4) color = ORANGE;

                DrawRectangleRec(blockDst, color);
            }
        }
    }
}

void Tetris::loadAllImages()
{
    DrawBackground();
    DrawTexture(introTexture, 100, 50, WHITE);
    DrawTexture(creditsTexture, 100, 150, WHITE);

    DrawPlayfield();          

    // Old test pieces (commented for now)
     loadTblock();
     loadZblock();
     loadSblock();
     loadLblock();
     loadJblock();
     loadOblock();
     loadIblock();
}

void Tetris::CreateIntro()
{
    int fontSize = 40;

    // Measure the text
    const char* msg = "Welcome to Tetris";
    int textW = MeasureText(msg, fontSize);
    int textH = fontSize;

    // Add huge left padding so the text appears on the right side
    int paddingLeft = 650;   // increase to push further right
    int paddingTop = 0;     // change to move up/down

    // Create a larger image
    Image img = GenImageColor(textW + paddingLeft, textH + paddingTop, BLANK);
    // Draw text inside the image at the padded position
    ImageDrawText(&img, msg, paddingLeft, paddingTop, fontSize, DARKBLUE);
    introTexture = LoadTextureFromImage(img);
    UnloadImage(img);
}

void Tetris::CreateCredits()
{
    int fontSize = 40;

    // Measure the text
    const char* msg = "Prototype: By Tony";
    int textW = MeasureText(msg, fontSize);
    int textH = fontSize;

    // Add huge left padding so the text appears on the right side
    int paddingLeft = 650;   // increase to push further right
    int paddingTop = 25;     // change to move up/down

    // Create a larger image
    Image img = GenImageColor(textW + paddingLeft, textH + paddingTop, BLANK);
    // Draw text inside the image at the padded position
    ImageDrawText(&img, msg, paddingLeft, paddingTop, fontSize, DARKBROWN);
    creditsTexture = LoadTextureFromImage(img);
    UnloadImage(img);
}

void Tetris::DrawBackground()
{
    Rectangle src = { 0, 0, (float)background.width, (float)background.height };
    Rectangle dst = { 0, 0, 1200, 900 };
    DrawTexturePro(background, src, dst, { 0,0 }, 0.0f, WHITE);
}

int Tetris::updateScore(int linesCleared)
{
    score += linesCleared * 100;
    return score;
}