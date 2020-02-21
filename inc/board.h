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
#ifdef DEFAULT_TETRIS_MODE_OFF
    Block blockShadow;
#endif
    SDL_Window *sWnd;
    SDL_Renderer *renderer;

public:
    Board();
    Board(SDL_Window *sWnd, SDL_Renderer *renderer);
    ~Board();

    void drawFrames();
    void drawCells();
#ifdef DEFAULT_TETRIS_MODE_OFF
    void drawBlockShadow();
    Block updateBlockShadow();
#endif
    void render();

    int keyboardHandle(SDL_KeyboardEvent *key);
    int fallCheck();
    uint checkLines();
    void clearLine(int lineN);
    void upperLayerDrop(int lineN);

    Block *newBlock();
};
