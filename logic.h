#pragma once
#include "tetris.h"

class Logic
{
public:
    Logic();

    int currentType;
    int nextType;

    int pieceX;
    int pieceY;

    int currentShape[4][4];

    int holdType;
    bool holdUsed;

    int score;
    int linesClearedTotal;
    int highScore;

    // NEW: level system
    int level;
    int linesToNextLevel;

    // NEW: 7-bag
    int bag[7];
    int bagIndex;
    void RefillBag();
    int PullFromBag();

    bool SpawnPiece(int board[20][10]);
    bool CheckCollisionDown(const int board[20][10]);
    bool CheckCollisionLeft(const int board[20][10]);
    bool CheckCollisionRight(const int board[20][10]);

    bool MoveDown(int board[20][10]);
    void MoveLeft(int board[20][10]);
    void MoveRight(int board[20][10]);
    void Rotate(int board[20][10]);
    void HardDrop(int board[20][10]);
    void HoldPiece(int board[20][10]);
    void LockPiece(int board[20][10]);
    int  ClearLines(int board[20][10]);
    void LoadHighScore();
    void SaveHighScore();
};
