#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL_render.h>
#include <SDL_rect.h>

#include "config.h"

extern Uint8 GRAY[3];
extern Uint8 BLACK[3];
extern Uint8 WHITE[3];
extern Uint8 *BLOCK_COLORS[7];

struct BlockType
{
    bool *boolFilter;
    int blockMatrLen;
    int blockDimension;

    BlockType()
    {
        boolFilter = nullptr;
    }

    BlockType(bool *type, int len = 9, int dim = 3)
    {
        boolFilter = type;
        blockMatrLen = len;
        blockDimension = dim;
    }

};

class Block
{
    BlockType type;
    SDL_Renderer *renderer;

    int position[2];
    int brushN;

    bool fake;
    bool *boolStatus;

public:

    Block()
    {
        boolStatus = nullptr;
        renderer = nullptr;
        position[0] = 4;
        position[1] = 0;
        brushN = 0;
    }

    Block(SDL_Renderer *renderer, bool *boolStatus);

    int *getBrushN();
    int *getPosition();
    BlockType *getType();

    void render();

    void freeze(int *boardColorStatus);
    bool falling();
    bool moving(int x_vector);
    bool check(int x_vector, int y_vector, bool *new_filter);
    bool rotate();

};
