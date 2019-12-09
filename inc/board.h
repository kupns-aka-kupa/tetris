#pragma once
#include "block.h"
#include <SDL_video.h>
#include <SDL_events.h>

typedef unsigned int uint;

class Board
{
    int *colorStatus;
    bool *boolStatus;
    Block *block;

    SDL_Window *sWnd;
    SDL_Renderer *renderer;

public:
    Board()
    {
        colorStatus = nullptr;
        boolStatus = nullptr;
        block = nullptr;
        sWnd = nullptr;
        renderer = nullptr;
    }

    Board(SDL_Window *sWnd, SDL_Renderer *renderer);

    ~Board()
    {
        delete []colorStatus;
        delete []boolStatus;
        delete block;
    }
    int get_cl();

    void drawFrames();
    void drawCells();
    void render();

    bool keyboardHandle(SDL_KeyboardEvent *key);
    bool fallCheck();
    uint checkLines();
    void clearLine(int lineN);
    void upperLayerDrop(int lineN);

    Block *newBlock();
};
