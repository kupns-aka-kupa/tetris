#include "Header.h"
#include "Board.h"

bool run = true;
using constants::size;
Board board (
	size * 10,
	size * 15,
	size,
	size
);

int main()
{
	board.new_block();
	board.init();
	while (run)
	{
		board.board_update();
	}
	return 0;
}

