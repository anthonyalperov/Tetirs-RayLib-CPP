#include "logic.h"
#include "tetris.h"

Logic::Logic()
{
    // NEW: init bag first
    bagIndex = 0;
    RefillBag();

    currentType = PullFromBag();
    nextType = PullFromBag();

    pieceX = 3;
    pieceY = 0;

    holdType = 0;
    holdUsed = false;

    score = 0;
    linesClearedTotal = 0;
    highScore = 0;

    // NEW: level system
    level = 1;
    linesToNextLevel = 10;

    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            currentShape[y][x] = 0;
}


void Logic::RefillBag()
{
    for (int i = 0; i < 7; i++)
        bag[i] = i + 1;

    for (int i = 0; i < 7; i++)
    {
        int j = GetRandomValue(0, 6);
        int tmp = bag[i];
        bag[i] = bag[j];
        bag[j] = tmp;
    }

    bagIndex = 0;
}

int Logic::PullFromBag()
{
    if (bagIndex >= 7)
        RefillBag();
    return bagIndex < 7 ? bag[bagIndex++] : 1;
}

bool Logic::SpawnPiece(int board[20][10])
{
    currentType = nextType;
    nextType = PullFromBag();   // NEW: use bag

    pieceX = 3;
    pieceY = 0;

    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            currentShape[y][x] = TETROMINOES[currentType - 1][y][x];

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (currentShape[y][x] == 1)
            {
                int bx = pieceX + x;
                int by = pieceY + y;

                if (board[by][bx] != 0)
                    return true;
            }
        }
    }
    holdUsed = false;
    return false;
}





bool Logic::CheckCollisionDown(const int board[20][10])
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (currentShape[y][x] == 1)
            {
                int bx = pieceX + x;
                int by = pieceY + y;

                if (by + 1 >= 20)
                    return true;

                if (board[by + 1][bx] != 0)
                    return true;
            }
        }
    }
    return false;
}

bool Logic::CheckCollisionLeft(const int board[20][10])
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (currentShape[y][x] == 1)
            {
                int bx = pieceX + x;
                int by = pieceY + y;

                if (bx - 1 < 0)
                    return true;

                if (board[by][bx - 1] != 0)
                    return true;
            }
        }
    }
    return false;
}

bool Logic::CheckCollisionRight(const int board[20][10])
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (currentShape[y][x] == 1)
            {
                int bx = pieceX + x;
                int by = pieceY + y;

                if (bx + 1 >= 10)
                    return true;

                if (board[by][bx + 1] != 0)
                    return true;
            }
        }
    }
    return false;
}

bool Logic::MoveDown(int board[20][10])
{
    if (!CheckCollisionDown(board))
    {
        pieceY++;
        score += 1; // soft drop point
        return true;
    }
    return false;
}

void Logic::MoveLeft( int board[20][10])
{
    if (!CheckCollisionLeft(board))
        pieceX--;
}

void Logic::MoveRight( int board[20][10])
{
    if (!CheckCollisionRight(board))
        pieceX++;
}

void Logic::Rotate(int board[20][10])
{
    int rotated[4][4] = {0};

	if (currentType == 6) // O piece doesn't rotate
        return;

    // rotate clockwise
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            rotated[x][3 - y] = currentShape[y][x];

    // collision test
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (rotated[y][x] == 1)
            {
                int bx = pieceX + x;
                int by = pieceY + y;

                if (bx < 0 || bx >= 10 || by < 0 || by >= 20)
                    return;

                if (board[by][bx] != 0)
                    return;
            }
        }
    }

    // apply rotation
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            currentShape[y][x] = rotated[y][x];
}

void Logic::HardDrop(int board[20][10])
{
    int dropDistance = 0;

    while (!CheckCollisionDown(board))
    {
        pieceY++;
        dropDistance++;
    }

    score += dropDistance * 2;
    LockPiece(board);
}

void Logic::HoldPiece(int board[20][10])
{
    if (holdUsed)
        return; // can't hold twice before locking

    int oldType = currentType;

    if (holdType == 0)
    {
        // First time holding: store current piece
        holdType = oldType;

        // Use next piece
        currentType = nextType;
        nextType = GetRandomValue(1, 7);
    }
    else
    {
        // Swap current with hold
        int temp = holdType;
        holdType = oldType;
        currentType = temp;
    }

    // Reset position
    pieceX = 3;
    pieceY = 0;

    // ALWAYS reload the shape AFTER swapping
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            currentShape[y][x] = TETROMINOES[currentType - 1][y][x];

    holdUsed = true;
}










void Logic::LockPiece(int board[20][10])
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (currentShape[y][x] == 1)
            {
                int bx = pieceX + x;
                int by = pieceY + y;

                if (by >= 0 && by < 20 && bx >= 0 && bx < 10)
                    board[by][bx] = currentType;
            }
        }
    }
}

int Logic::ClearLines(int board[20][10])
{
    int linesCleared = 0;

    for (int y = 0; y < 20; y++)
    {
        bool full = true;
        for (int x = 0; x < 10; x++)
        {
            if (board[y][x] == 0)
            {
                full = false;
                break;
            }
        }

        if (full)
        {
            linesCleared++;

            for (int row = y; row > 0; row--)
                for (int col = 0; col < 10; col++)
                    board[row][col] = board[row - 1][col];

            for (int col = 0; col < 10; col++)
                board[0][col] = 0;
        }
    }

    linesClearedTotal += linesCleared;

    // Score (you can later multiply by level if you want)
    switch (linesCleared)
    {
    case 1: score += 100; break;
    case 2: score += 300; break;
    case 3: score += 500; break;
    case 4: score += 800; break;
    }

    // NEW: level progression
    linesToNextLevel -= linesCleared;
    if (linesToNextLevel <= 0)
    {
        level++;
        linesToNextLevel += 10;
    }

    return linesCleared;
}


void Logic::LoadHighScore()
{
    FILE* file = nullptr;
    fopen_s(&file, "highscore.dat", "rb");    
    if (file)
    {
        fread(&highScore, sizeof(int), 1, file);
        fclose(file);
    }
}

void Logic::SaveHighScore()
{
    FILE* file = nullptr;
    fopen_s(&file, "highscore.dat", "wb");
    if (file)
    {
        fwrite(&highScore, sizeof(int), 1, file);
        fclose(file);
    }
}