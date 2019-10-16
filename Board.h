#pragma once
#include "Block.h"

class Board
{
public:
	Board()
	{
		width = 250; 
		height = 500;
		offset_x = 50;
		offset_y = 50;
		board_status = NULL;
	}

	Board(int w, int h, int offset_x, int offset_y)
	{
		width = w; 
		height = h;
		this->offset_x = offset_x;
		this->offset_y = offset_y;
	}

	void draw_board();
	void fall_check();
	bool *init();
	void board_update();
	Block *add_freezed();
	Block *new_block();
	bool *clear_lines();
	bool check_is_empty_block(bool expression, int k);

private:
	int freezed_count;
	int width, height, offset_x, offset_y;
	int m, n;
	bool *board_status;
	Block **freezed;
	Block *block;
};