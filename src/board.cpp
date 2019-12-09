#include "board.h"

Board::Board(SDL_Window *sWnd, SDL_Renderer *renderer)
{
    this->sWnd = sWnd;
    this->renderer = renderer;
    colorStatus = new int [RC * CC];
    boolStatus = new bool [RC * CC];
    for (int i = 0; i < RC * CC; i ++)
    {
        colorStatus[i] = 0;
        boolStatus[i] = 0;
    }
}

Block *Board::newBlock()
{
    block = new Block(renderer, boolStatus);
    return block;
}

bool Board::keyboardHandle(SDL_KeyboardEvent *key)
{
    if(key->keysym.sym == SDLK_w || key->keysym.sym == SDLK_UP)
    {
        block->rotate();
    }
    else if (key->keysym.sym == SDLK_SPACE)
    {
        while (fallCheck()) {}
        return true;
    }
    else if (key->keysym.sym == SDLK_a || key->keysym.sym == SDLK_LEFT)
    {
        block->moving(-1);
    }
    else if (key->keysym.sym == SDLK_d || key->keysym.sym == SDLK_RIGHT)
    {
        block->moving(1);
    }
    else if (key->keysym.sym == SDLK_s || key->keysym.sym == SDLK_DOWN)
    {
        return fallCheck();
    }
    return 0;
}

void Board::drawFrames()
{
    SDL_SetRenderDrawColor(renderer, GRAY[0], GRAY[1], GRAY[2], 0xFF);
    SDL_Rect main MAIN_FRAME;
    SDL_RenderFillRect(renderer, &main);

    SDL_Rect mainframe TETRIS_FRAME;
    SDL_SetRenderDrawColor(renderer, WHITE[0], WHITE[1], WHITE[2], 0xFF);
    SDL_RenderDrawRect(renderer, &mainframe);

    SDL_Rect gameInfoFrame GAME_INFO_FRAME;
    SDL_SetRenderDrawColor(renderer, WHITE[0], WHITE[1], WHITE[2], 0xFF);
    SDL_RenderDrawRect(renderer, &gameInfoFrame);

    SDL_Rect newBlockInfoFrame BLOCK_INFO_FRAME;
    SDL_SetRenderDrawColor(renderer, WHITE[0], WHITE[1], WHITE[2], 0xFF);
    SDL_RenderDrawRect(renderer, &newBlockInfoFrame);
}

void Board::drawCells()
{
    for(int i = 0; i < RC; i ++)
    {
        for(int j = 0; j < CC; j ++)
        {
            if(boolStatus[i * CC + j])
            {
                SDL_Rect cell{FO + j * BLC_SCL, FO + i * BLC_SCL, BLC_SCL, BLC_SCL};
                SDL_SetRenderDrawColor(
                        renderer,
                        BLOCK_COLORS[colorStatus[i * CC + j]][0],
                        BLOCK_COLORS[colorStatus[i * CC + j]][1],
                        BLOCK_COLORS[colorStatus[i * CC + j]][2],
                        0xFF
                        );
                SDL_RenderFillRect(renderer, &cell);
                SDL_SetRenderDrawColor(renderer, WHITE[0], WHITE[1], WHITE[2], 0xFF);
                SDL_RenderDrawRect(renderer, &cell);
            }
        }
    }
}

void Board::render()
{
    drawFrames();
    drawCells();
    block->render();
}

void Board::upperLayerDrop(int lineN)
{
    if(lineN > 0){
        for(int j = 0; j < CC; j++)
        {
            boolStatus[lineN * CC + j] = boolStatus[(lineN - 1) * CC + j];
            colorStatus[lineN * CC + j] = colorStatus[(lineN - 1) * CC + j];
        }
        upperLayerDrop(lineN - 1);
    }
}

bool Board::fallCheck()
{
    if (!block->falling())
    {
        block->freeze(colorStatus);
        newBlock();
        return false;
    }
    else
    {
        return true;
    }
}

uint Board::checkLines()
{
    uint clearedLines = 0;
    for (int i = 0; i < RC; i++)
    {
        bool filled = true;
        for (int j = 0; j < CC; j++)
        {
            if(!boolStatus[i * CC + j])
            {
                filled = false;
                break;
            }
        }
        if (filled)
        {
            clearedLines += 1;
            clearLine(i);
        }
    }
    return clearedLines;
}

void Board::clearLine(int lineN)
{
    for(int j = 0; j < CC; j++)
    {
        boolStatus[lineN * CC + j] = 0;
    }
    upperLayerDrop(lineN);
}
