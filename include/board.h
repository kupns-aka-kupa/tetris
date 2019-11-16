#pragma once
#include "view.h"
#include "events.h"
#include "block.h"

class Board
{
    int freezed_count;
    int width, height;
    int m, n;
    bool *board_status;
    Block **freezed;
    Block *block;

public:
    Board()
    {
        width = constants::board_width;
        height = constants::board_height;
        m = constants::row_count;
        n = constants::col_count;
        board_status = nullptr;
    }

    void draw_board();
    void fall_check();
    bool *init();
    void board_update();
    Block *add_freezed();
    Block *new_block();
    bool *clear_lines();
    bool check_is_empty_block(bool expression);
};
