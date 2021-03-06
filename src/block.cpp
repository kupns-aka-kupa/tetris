#include "block.h"

Uint8 GRAY[3] = {0x31, 0x36, 0x3b};
Uint8 BLACK[3] = {0x23, 0x26, 0x2a};
Uint8 WHITE[3] = {0xef, 0xf0, 0xf1};

static Uint8 RED[3] = {0xce, 0x42, 0x50};
static Uint8 ORANGE[3] = {0xf5, 0x9e, 0x16};
static Uint8 BLUE[3] = {0x29, 0x80, 0xb9};
static Uint8 GREEN[3] = {0x26, 0x8C, 0x52};
static Uint8 MAGENTA[3] = {0xbd, 0x93, 0xf9};
static Uint8 PURPLE[3] = {0xf6, 0x76, 0xc0};

static bool T_block[9] = {0, 1, 0, 1, 1, 1, 0, 0, 0};
static bool Ll_block[9] = {1, 0, 0, 1, 1, 1, 0, 0, 0};
static bool Lr_block[9] = {0, 0, 1, 1, 1, 1, 0, 0, 0};
static bool Zl_block[9] = {0, 1, 1, 1, 1, 0, 0, 0, 0};
static bool Zr_block[9] = {1, 1, 0, 0, 1, 1, 0, 0, 0};
static bool I_block[16] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
static bool Sq_block[4] = {1, 1, 1, 1};

static BlockType BLOCK_TYPES[7]
{
    BlockType(T_block),
    BlockType(Ll_block),
    BlockType(Lr_block),
    BlockType(Zl_block),
    BlockType(Zr_block),
    BlockType(Sq_block, 4, 2),
    BlockType(I_block, 16, 4),
};

#ifdef DEFAULT_TETRIS_MODE_OFF

enum BLOCK_MODIFIERS{
    STANDART,
    INVERTED,
    BLOWUP,
    FAKE,
    FLUID
};

Uint8 *BLOCK_COLORS[8] = {
    RED,
    ORANGE,
    BLUE,
    GREEN,
    MAGENTA,
    PURPLE,
    BLACK,
    WHITE
};

#else

Uint8 *BLOCK_COLORS[6] = {
    RED,
    ORANGE,
    BLUE,
    GREEN,
    MAGENTA,
    PURPLE
};

#endif

void Block::setBrush(int brushN)
{
    this->brushN = brushN;
}
#include <iostream>
Block::Block(SDL_Renderer *renderer, bool *boardBoolStatus)
{
    this->renderer = renderer;
    boolStatus = boardBoolStatus;

    int bCLen = sizeof(BLOCK_COLORS) / sizeof(&BLOCK_COLORS) - 1;
    int bTLen = sizeof(BLOCK_TYPES) / sizeof(BLOCK_TYPES[0]);
    int typeIndex = std::rand() % bTLen;

    std::srand(unsigned(std::time(nullptr)));
    brushN = std::rand() % bCLen;
#ifdef DEFAULT_TETRIS_MODE_OFF
    if (brushN != 6) modifier = STANDART;
    else modifier = std::rand() % (sizeof(BLOCK_MODIFIERS) - 1) + 1;
#endif
    type = BlockType(
        BLOCK_TYPES[typeIndex].boolFilter,
        BLOCK_TYPES[typeIndex].blockMatrLen,
        BLOCK_TYPES[typeIndex].blockDimension);
    position[0] = int(CC / 2);
    position[1] = 0;
}

Block::Block(const Block *copy)
    : type(copy->type),
      renderer(copy->renderer),
      position{copy->position[0], copy->position[1]},
      brushN(copy->brushN),
      boolStatus(copy->boolStatus){}

void Block::drawCell(SDL_Rect *cell)
{
    SDL_SetRenderDrawColor(renderer, BLOCK_COLORS[brushN][0], BLOCK_COLORS[brushN][1], BLOCK_COLORS[brushN][2], 0xFF);
    SDL_RenderFillRect(renderer, cell);
    SDL_SetRenderDrawColor(renderer, WHITE[0], WHITE[1], WHITE[2], 0xFF);
    SDL_RenderDrawRect(renderer, cell);
}

void Block::renderPrewiew()
{
    int nBIF[4] = BLOCK_INFO_FRAME;
    int d = type.blockDimension;
    render(nBIF[0] + (BLOCK_IFNO_PANEL_WIDTH / d / 2) + FO / 2,
           nBIF[1] + (BLOCK_IFNO_PANEL_HEIGHT / d / 2) + FO / 2,
           false);
}

void Block::render(int x0, int y0, bool dynamic)
{
    for(int i = 0; i < type.blockDimension; i++)
    {
        for (int j = 0; j < type.blockDimension; j++)
        {
            if (type.boolFilter[i * type.blockDimension + j])
            {
                SDL_Rect cell;
                if (dynamic)
                {
                    cell = SDL_Rect{
                        x0 + (j + position[0]) * BLC_SCL,
                        y0 + (i + position[1]) * BLC_SCL,
                        BLC_SCL,
                        BLC_SCL
                    };
                }
                else
                {
                    cell = SDL_Rect{
                        x0 + j * BLC_SCL,
                        y0 + i * BLC_SCL,
                        BLC_SCL,
                        BLC_SCL
                    };
                }
                drawCell(&cell);
            }
        }
    }
}

bool Block::check(int x_vector, int y_vector, bool *newFilter)
{
/**
 *  One function to check all possible interactions with the board
 *      1) turns,
 *      2) movement along the axes "x" and "y";
 *      taking into account the state of the board.
 *
 *  There will be a check of the new position (bool *newFilter) relative to the board
 *      1) occupied cells,
 *      2) out of the board;
 *      guaranteed suppression of movement through the left, right and bottom borders,
 *      according to the movement vector (int x_vector), (int y_vector)
 *
 *  (int y_vector) can only be equal int(1),
 *      while (int x_vector)-----> int(1)
 *                             \
 *                              -> int(-1)
 *
 *  Below in the form of links will be an explanation of each expression in the body
 *      of that function:
 *
 * /- [!]: The whole algorithm is similar to projecting the coordinates of a block
 * |    on a board and comparing fingerprints,
 * |    for this you need to start only from "true" values in (bool *newFilter),
 * |    so that it becomes possible to move not just a block_frame (3 * 3) or (4 * 4),
 * |    but its values themselves,
 * |    because the frame can crawl out onto the borders of the block,
 * |    but value no.
 * |
 * |-> Mapping values (bool *newFilter) onto a board,
 * |    prevents movement through blocks,
 * |    that have already standing on the:
 * |    [0]: sides,
 * |    [1]: bottom,
 * \
 *  -> Prevents movement through board sides:
 *      [2]: bottom,
 *      [3]: left,
 *      [4]: right;
 *
 *  It’s easy to guess if expressions [0 - 4] == "false",
 *  the function returns "false",
 *  thereby blocking the movement depending on the context
 *
 */
    for (int i = 0; i < type.blockDimension; i++)
    {
        for (int j = 0; j < type.blockDimension; j++)
        {
            if (
                /*[!]:*/newFilter[i * type.blockDimension + j]
                &&
                (
                    /*[0]:*/boolStatus[(i + position[1]) * CC + j + x_vector + position[0]] == true
                    ||
                    /*[1]:*/boolStatus[(i + y_vector + position[1]) * CC + j + position[0]] == true
                    ||
                    /*[3]:*/(i + y_vector + position[1]) * CC + j + position[0] > CC * RC
                    ||
                    /*[4]:*/j + x_vector + position[0] < 0
                    ||
                    /*[5]:*/j + x_vector + position[0] > CC - 1
                )
            )
            {
                return 0;
            }
        }
    }
    return 1;
}

bool Block::rotate()
{
    bool *newFilter = new bool[type.blockMatrLen];

    for (int  i = 0; i < type.blockDimension ; i ++)
    {
        for (int  j = 0; j < type.blockDimension ; j ++)
        {
            newFilter[i * type.blockDimension + j] = type.boolFilter[j * type.blockDimension + type.blockDimension - i - 1];
        }
    }

    if (check(0, 0, newFilter)) {
        for (int  i = 0; i < type.blockDimension ; i ++)
        {
            for (int  j = 0; j < type.blockDimension ; j ++)
            {
                type.boolFilter[i * type.blockDimension + j] = newFilter[i * type.blockDimension + j];
            }
        }
        delete []newFilter;
        return 1;
    }
    else
    {
        delete []newFilter;
        return 0;
    }
}

bool Block::falling()
{
    if (check(0, 1, type.boolFilter)) return position[1] += 1;
    else return 0;
}

bool Block::moving(int x_vector)
{
    if (check(x_vector, 0, type.boolFilter)) return position[0] += x_vector;
    else return 0;
}

bool Block::freeze(int *boardColorStatus)
{
#ifdef DEFAULT_TETRIS_MODE_OFF
    if (modifier == FAKE) return true;
#endif
    for (int i = 0; i < type.blockDimension; i++)
    {
        for (int j = 0; j < type.blockDimension; j++)
        {
            bool cell = type.boolFilter[i * type.blockDimension + j];
            if (i + position[1] < 4) return false;
#ifdef DEFAULT_TETRIS_MODE_OFF
            else if (modifier == BLOWUP)
            {
                boolStatus[(i + position[1]) * CC + j + position[0]] = 0;
                boardColorStatus[(i + position[1]) * CC + j + position[0]] = 0;
            }
#endif
            else
            {
                if (cell)
                {
                    if (modifier == STANDART)
                    {
                        setBoardCell(i, j, boardColorStatus);
                    }
#ifdef DEFAULT_TETRIS_MODE_OFF
                    else if (modifier == FLUID)
                    {
                        int row = i;
                        while(row + position[1] + 1 <= RC && !boolStatus[(row + position[1]) * CC + j + position[0]])
                        {
                            setBoardCell(row, j, boardColorStatus);
                            row++;
                        }
                    }
#endif
                }
#ifdef DEFAULT_TETRIS_MODE_OFF
                else
                {
                    if (modifier == INVERTED)
                    {
                        setBoardCell(i, j, boardColorStatus);
                    }
                }
#endif
            }
        }
    }
    return true;
}

void Block::setBoardCell(int row, int collumn, int *boardColorStatus)
{
    boolStatus[(row + position[1]) * CC + collumn + position[0]] = 1;
    boardColorStatus[(row + position[1]) * CC + collumn + position[0]] = brushN;
}
