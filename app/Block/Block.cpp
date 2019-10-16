#include "../Header/Header.h"
#include "Block.h"

using constants::hDC;
using constants::size;

HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
HBRUSH hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
HBRUSH hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
HBRUSH hPurpleBrush = CreateSolidBrush(RGB(255, 0, 255));
HBRUSH hYellowBrush = CreateSolidBrush(RGB(255, 255, 0));
HBRUSH hCyanBrush = CreateSolidBrush(RGB(0, 255, 255));

bool T_block[9] = { 0, 1, 0, 1, 1, 1, 0, 0, 0 };
bool Ll_block[9] = { 1, 0, 0, 1, 1, 1, 0, 0, 0 };
bool Lr_block[9] = { 0, 0, 1, 1, 1, 1, 0, 0, 0 };
bool Zl_block[9] = { 0, 1, 1, 1, 1, 0, 0, 0, 0 };
bool Zr_block[9] = { 1, 1, 0, 0, 1, 1, 0, 0, 0 };
bool I_block[16] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
bool Sq_block[4] = { 1, 1, 1, 1 };

BlockType block_types[7]
{
	BlockType(T_block),
	BlockType(Ll_block),
	BlockType(Lr_block),
	BlockType(Zl_block),
	BlockType(Zr_block),
	BlockType(Sq_block, 4, 2),
	BlockType(I_block, 16, 4),
};

HBRUSH hBrushes[6] = {
	hRedBrush,
	hGreenBrush,
	hBlueBrush,
	hPurpleBrush,
	hYellowBrush,
	hCyanBrush
};

BlockType::BlockType(bool* type, int len, int delim)
{
	bool_filter = new bool[len];
	for (int i = 0; i < len; i++)
	{
		bool_filter[i] = type[i];
	}
	block_len = len;
	block_delim = delim;
}

int *Block::get_position()
{
	return position;
}

BlockType *Block::get_type()
{
	return &type;
}

BlockType Block::init()
{
	int hBrushesLen = sizeof(hBrushes) / sizeof(&hBrushes);
	int block_filter_len = sizeof(block_types) / sizeof(block_types[0]);
	std::srand(unsigned(std::time(0)));
	Brush = hBrushes[std::rand() % hBrushesLen];
	int type_index = std::rand() % block_filter_len;
	type = BlockType(
		block_types[type_index].bool_filter,
		block_types[type_index].block_len,
		block_types[type_index].block_delim
		);
	return type;
}

void Block::draw_block(int x_0, int y_0)
{
	SelectObject(hDC, Brush);
	SelectObject(hDC, GetStockObject(BLACK_PEN));

	for(int i = 0; i < type.block_delim; i++)
	{	
		for (int j = 0; j < type.block_delim; j++)
		{
			if (type.bool_filter[i * type.block_delim + j])
			{
				RECT rect = { 
					x_0 + (j + position[0]) * size,
					y_0 + (i + position[1]) * size,
					x_0 + (j + position[0] + 1) * size,
					y_0 + (i + position[1] + 1) * size
				};
				Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
			}
		}
	}
}

bool Block::check_availability(int x_vector, bool* filter, bool* board_status, int n, int y_vector)
{
	for (int i = 0; i < type.block_delim; i++)
	{
		for (int j = 0; j < type.block_delim; j++)
		{
			if (
				filter[i * type.block_delim + j]
				&&
				(
					board_status[(i + y_vector + position[1]) * n + j + position[0]]
					||
					board_status[(i + position[1]) * n + j + x_vector + position[0]]
					||
					(
						j + x_vector + position[0] < 0
						||
						j + x_vector + position[0] > n - 1
					)
				)
			)
			{
				return 0;
			}
		}
	}
	return 1;
}

bool Block::rotate(bool* board_status, int n)
{	
	bool *new_filter = new bool[type.block_len];
	
	for (int  i = 0; i < type.block_delim ; i ++)
	{
		for (int  j = 0; j < type.block_delim ; j ++)
		{
			new_filter[i * type.block_delim + j] = type.bool_filter[j * type.block_delim + type.block_delim - i - 1];
		}
	}

	if (check_availability(0, new_filter, board_status, n, 0)) {
		for (int  i = 0; i < type.block_delim ; i ++)
		{
			for (int  j = 0; j < type.block_delim ; j ++)
			{
				type.bool_filter[i * type.block_delim + j] = new_filter[i * type.block_delim + j];
			}
		}
		delete[] new_filter;
		return type.bool_filter;
	}
	else
	{
		delete[] new_filter;
		return 0;
	}
}

int Block::falling(bool* board_status, int n)
{
	if (check_availability(0, type.bool_filter, board_status, n , 1))
	{
		return this->position[1] += 1;
	}
	else
	{
		return 0;
	}
}

int Block::moving(int vector, bool* board_status, int n)
{
	if (check_availability(vector, type.bool_filter, board_status, n, 0))
	{
		return this->position[0] += vector;
	}
	else
	{
		return this->position[0];
	}
}

void Block::block_freeze(bool* board_status, int n)
{	
	for (int i = 0; i < type.block_delim; i++)
	{
		for (int j = 0; j < type.block_delim; j++)
		{
			board_status[(i + position[1]) * n + j + position[0]] =
				board_status[(i + position[1]) * n + j + position[0]]
				||
				type.bool_filter[i * type.block_delim + j];
		}
	}
}
