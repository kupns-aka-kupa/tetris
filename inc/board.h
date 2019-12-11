#pragma once
#include "block.h"
#include <SDL_video.h>
#include <SDL_events.h>

typedef unsigned int uint;

class Board
{
    int *colorStatus;
    bool *boolStatus;
    Block *currentBlock;
    Block *nextBlock;

    SDL_Window *sWnd;
    SDL_Renderer *renderer;

public:
    Board();
    Board(SDL_Window *sWnd, SDL_Renderer *renderer);
    ~Board();

    int get_cl();

    void drawFrames();
    void drawCells();
    void render();

    int keyboardHandle(SDL_KeyboardEvent *key);
    int fallCheck();
    uint checkLines();
    void clearLine(int lineN);
    void upperLayerDrop(int lineN);

    Block *newBlock();
};
