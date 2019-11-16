#include "board.h"

using namespace constants;

//static Events events;

Block *Board::new_block()
{
    block = new Block(4, 0);
    block->init();
    return block;
}

void Board::board_update()
{/*
    width = hDC->w;
    height = hDC->h;*/
//	bool* KEY = events.get_key();
//	if (KEY[32] || KEY[38] || KEY[87])//space || up_arrow || w
//	{
//		block->rotate(board_status, n);
//	}
//	else if (KEY[37] || KEY[65])//left
//	{
//		block->moving(-1, board_status, n);
//	}
//	else if (KEY[39] || KEY[68])//right
//	{
//		block->moving(1, board_status, n);
//	}
//	else if (KEY[40] || KEY[83])//down
//	{
//		fall_check();
//	}
//	fall_check();
//    SDL_Delay(300);
    draw_board();
}

void Board::fall_check()
{
//    if (block->falling(board_status, n) == 0)
//    {
        //if (block->get_position()[1] < 4 )
        //{
        //	for (int k = 0; k < freezed_count; k++)
        //	{
        //		check_is_empty_block(true, k);
        //	}
        //}

//        block->block_freeze(board_status, n);
//        add_freezed();
//        clear_lines();
//        new_block();
//    }
}

void Board::draw_board()
{
    SDL_Rect main{offset_x, offset_y, width, height};
    SDL_FillRect(hDC, &main, SDL_MapRGB( hDC->format, 0xFF, 0xFF, 0xFF ));
//    block->draw_block(offset_x, offset_y);
//	for (int k = 0; k < freezed_count; k++)
//	{
//		freezed[k]->draw_block(offset_x, offset_y);
//	}
    for(int i = 0; i < m; i ++)
    {
        for(int j = 0; j < n; j ++)
        {
            if(board_status[i * n + j])
            {
                SDL_Rect cell{
                    offset_x + j * pSize,
                    offset_y + i * pSize,
                    offset_x + (1 + j) * pSize,
                    offset_y + (1 + i) * pSize
                };
                SDL_FillRect(hDC, &cell, SDL_MapRGB( hDC->format, 0xff, 0x00, 0x00));
            }
        }
    }
    SDL_UpdateWindowSurface(hWnd);
}

bool *Board::init (){
	board_status = new bool [(m + 1) * n];
	for (int i = 0; i < m * n; i ++)
	{
		board_status[i] = 0;
	}
    board_status[20] = 1;

	return board_status;
}

Block *Board::add_freezed()
{
//    freezed_count += 1;
//    Block** new_freezed = new Block*[freezed_count];
//    for (int i = 0; i < freezed_count - 1; i++)
//    {
//        new_freezed[i] = freezed[i];
//    }
//    new_freezed[freezed_count - 1] = block;
//    delete []freezed;
//    freezed = new_freezed;
    return *freezed;
}

bool *Board::clear_lines()
{
//    bool *filled = new bool;
//    for (int i = 0; i < m; i++)
//    {
//        *filled = true;
//        for (int j = 0; j < n; j++)
//        {
//            if(!board_status[i * n + j])
//            {
//                *filled = false;
//                break;
//            }
//        }

//        if (*filled)
//        {
//            init();
//            for (int k = 0; k < freezed_count; k++)
//            {
//                int *position = freezed[k]->get_position();
//                BlockType *this_type = freezed[k]->get_type();

//                bool *is_empty = new bool(true);
//                if(position[1] <= i && i <= position[1] + this_type->block_delim)
//                {
//                    for (int l = 0; l < this_type->block_delim; l++)
//                    {
//                        this_type->bool_filter[(i - position[1]) * this_type->block_delim + l] = 0;
//                    }
//                }

//                if(!check_is_empty_block(
//                    freezed[k]->falling(board_status, n) == 0, k)
//                )
//                {
//                    freezed[k]->block_freeze(board_status, n);
//                }
//            }
//        }
//    }

//    delete filled;
    return board_status;
}

bool Board::check_is_empty_block(bool expression)
{
	if(expression){
		//delete freezed[k];
		//add_freezed();
		return true;
	}
	else
	{
		return false;
	}
}
