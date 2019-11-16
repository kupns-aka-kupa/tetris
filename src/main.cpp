#include "view.h"
#include "board.h"

using namespace constants;

static SDL_Event event;
static bool run = true;
static Board board;

int main()
{
    board.new_block();
    board.init();
    while (run)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_QUIT)
            {
                run = false;
            }
        }
        board.board_update();
    }
    return 0;
}
