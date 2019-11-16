#include "view.h"

namespace constants
{
    const int row_count = 15, col_count = 10;
    const int SCREEN_WIDTH = row_count * 40, SCREEN_HEIGHT = col_count * 40;
    const int offset_x = SCREEN_WIDTH / 50, offset_y = SCREEN_WIDTH / 50;

    const int board_width = SCREEN_WIDTH / 2 - 2 * offset_x;
    const int board_height = SCREEN_HEIGHT - 2 * offset_y;
    const int pSize = board_height / row_count;

    static Uint32 flags =
            SDL_WINDOW_SHOWN |
            SDL_WINDOW_INPUT_FOCUS |
            SDL_WINDOW_RESIZABLE;

    SDL_Window *hWnd = SDL_CreateWindow(
                "Tetris",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                flags
            );

    SDL_Surface *hDC = SDL_GetWindowSurface(hWnd);
}
