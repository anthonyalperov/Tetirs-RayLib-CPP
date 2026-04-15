#pragma once
#include "raylib.h"
#include <iostream>
#include <string>

#define MAXSIZE_X 30
#define MAXSIZE_Y 30
#define BLOCK_SIZE_X 75
#define BLOCK_SIZE_Y 75

// Grid constants
constexpr int GRID_WIDTH = 10;
constexpr int GRID_HEIGHT = 20;
constexpr int BLOCK_SIZE = 40;
constexpr float SCALE = 1.5f;

class Tetris
{
protected:
    
	//Board representation (0 = empty, 1-7 = different block types)
    int board[GRID_HEIGHT][GRID_WIDTH] = { 0 };

	//point differentiators
    int score;
    int Tblock;
    int Zblock;
    int Sblock;
    int Lblock;
    int Jblock;
    int Oblock;
    int Iblock;

	//Coordinates for drawing
    float positionX;
    float positionY;


	//Texture Inizialization
    Texture2D background;
    Texture2D texT;
    Texture2D texZ;
    Texture2D texS;
    Texture2D texL;
    Texture2D texJ;
    Texture2D texO;
    Texture2D texI;
    Texture2D introTexture;
    Texture2D creditsTexture;
    Texture2D borderTexture;
    Texture2D gridTexture;

public:
    Tetris()
        : score(0), Tblock(0), Zblock(0), Sblock(0), Lblock(0),
        Jblock(0), Oblock(0), Iblock(0),
        positionX(0.0f), positionY(0.0f),
        background{}, texT{}, texZ{} {
    }

    ~Tetris() = default;

    // getters
    int getScore() const { return score; }
    int getTblock() const { return Tblock; }
    int getZblock() const { return Zblock; }
    int getSblock() const { return Sblock; }
    int getLblock() const { return Lblock; }
    int getJblock() const { return Jblock; }
    int getOblock() const { return Oblock; }
    int getIblock() const { return Iblock; }
    float getPositionX() const { return positionX; }
    float getPositionY() const { return positionY; }
    Texture2D getBackground() const { return background; }

    // setters
    void setScore(int newScore) { score = newScore; }
    void setTblock(int newTblock) { Tblock = newTblock; }
    void setZblock(int newZblock) { Zblock = newZblock; }
    void setSblock(int newSblock) { Sblock = newSblock; }
    void setLblock(int newLblock) { Lblock = newLblock; }
    void setJblock(int newJblock) { Jblock = newJblock; }
    void setOblock(int newOblock) { Oblock = newOblock; }
    void setIblock(int newIblock) { Iblock = newIblock; }
    void setPositionX(float newPositionX) { positionX = newPositionX; }
    void setPositionY(float newPositionY) { positionY = newPositionY; }
    void setBackground(const Texture2D& newBackground) { background = newBackground; }

    // resource management
    void InitResources(const std::string& basePath);
    void UnloadResources();

    // drawing/text 
    void CreateIntro();
    void CreateCredits();
    void DrawBackground();
    void DrawPlayfield();


    //loaders
    void loadTblock();
    void loadZblock();
    void loadSblock();
    void loadLblock();
    void loadJblock();
    void loadOblock();
    void loadIblock();
    void loadBorder();
    void loadAllImages();


        // New

    // logic
    int updateScore(int linesCleared);
};