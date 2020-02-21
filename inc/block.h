#pragma once
#include <cstdlib>
#include <ctime>
#include <SDL_render.h>
#include <SDL_rect.h>

#include "config.h"

extern Uint8 GRAY[3];
extern Uint8 BLACK[3];
extern Uint8 WHITE[3];

#ifdef DEFAULT_TETRIS_MODE_OFF
    extern Uint8 *BLOCK_COLORS[8];
#else
    extern Uint8 *BLOCK_COLORS[6];
#endif

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

    ~BlockType()
    {
        boolFilter = nullptr;
    }

};

class Block
{
    BlockType type;
    SDL_Renderer *renderer;

    int position[2];
    int brushN;
    bool *boolStatus;

#ifdef DEFAULT_TETRIS_MODE_OFF
    int modifier;
#endif

public:

    Block()
    {
        boolStatus = nullptr;
        renderer = nullptr;
    }

    Block(SDL_Renderer *renderer, bool *boolStatus);
    Block(const Block *renderer);

    ~Block()
    {
        boolStatus = nullptr;
        renderer = nullptr;
    }

    int *getBrushN();
    int *getPosition();
    BlockType *getType();
    void setBrush(int brushN);

    void coordsMapping(int x0 = FO, int y0 = FO, bool dynamic = true);
    void render(SDL_Rect *cell);
    void renderStatic(int x0, int y0);
    void flow(int row, int collumn, int *boardColorStatus);

    bool freeze(int *boardColorStatus);
    bool falling();
    bool moving(int x_vector);
    bool check(int x_vector, int y_vector, bool *new_filter);
    bool rotate();

};
