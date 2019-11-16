#pragma once
#include "view.h"

struct BlockType 
{
	bool *bool_filter;
	int block_len;
	int block_delim;

	BlockType()
	{
        bool_filter = nullptr;
		block_len = 9;
		block_delim = 3;
	}

	BlockType(bool *type, int len = 9, int delim = 3);

};

class Block
{
    int position[2];
    BlockType type;
    int Brush;

public:

	Block(int x = 4, int y = 0)
	{
		position[0] = x;
		position[1] = y;
	}

	int *get_position();
	BlockType *get_type();

	void draw_block(int x_0, int y_0);
	void block_freeze(bool* board_status, int n);

	int falling(bool* board_status, int n);
	int moving(int vector, bool* board_status, int n);

	bool check_availability(int x_vector, bool *filter, bool* board_status, int n, int y_vector);
	bool rotate(bool* board_status, int n);

    BlockType init();
};
