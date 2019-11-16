#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

namespace constants
{
    extern const int row_count;
    extern const int col_count;
    extern const int board_width;
    extern const int board_height;
    extern const int info_panel_width;
    extern const int info_panel_height;

    extern const int pSize;
    extern const int offset_x;
    extern const int offset_y;

    extern const int SCREEN_WIDTH;
    extern const int SCREEN_HEIGHT;
    extern SDL_Window *hWnd;
    extern SDL_Surface *hDC;
}
