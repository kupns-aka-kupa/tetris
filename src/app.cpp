#include "app.h"

bool App::init(){

    if(SDL_Init(SDL_FLAGS) == 0 && TTF_Init() == 0
#ifndef AUDIO_OFF
            &&
            Mix_Init(MIX_INIT_MP3) > 0
#endif
            )
    {
        sWnd = SDL_CreateWindow(
                    PROJECT_NAME,
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    SCREEN_WIDTH, SCREEN_HEIGHT,
                    SWND_FLAGS
                );

        score = 0;
        complexityLevel = 1;

#ifndef AUDIO_OFF
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
        musicTheme = Mix_LoadMUS( "../static/music/maintheme.mp3" );
#endif

        font = TTF_OpenFont("../static/fonts/From Cartoon Blocks.ttf", FONT_PT_SIZE);
        renderer = SDL_CreateRenderer(sWnd, -1, SDL_RENDERER_ACCELERATED);

        SDL_Surface *icon = IMG_Load("../static/ico/icon.ico");
        SDL_SetWindowIcon(sWnd, icon);

        board = new Board(sWnd, renderer);
        run = true;

        delete icon;
        return run;
    }
    else
    {
        return run;
    }
}

void App::gameInfoRender()
{
    char buf[5];
    const char *scrChar;
    if(score < 10) sprintf(buf, "%d%d%d%d", 0, 0, 0, score);
    else if (score >= 10 && score < 100) sprintf(buf, "%d%d%d", 0, 0, score);
    else if (score >= 100 && score < 1000) sprintf(buf, "%d%d", 0, score);
    else if (score >= 1000 && score < 10000) sprintf(buf, "%d", score);
    scrChar = buf;

    int gmf[4] = GAME_INFO_FRAME;
    SDL_Rect scl {gmf[0], gmf[1], gmf[2], gmf[3] / 4};
    textRender(" Score: ", &scl);
    SDL_Rect sc {scl.x, scl.y + scl.h, gmf[2], gmf[3] / 4};
    textRender(scrChar, &sc);
}

void App::textRender(const char *text, SDL_Rect *dstrect)
{
    SDL_Color color = {WHITE[0], WHITE[1], WHITE[2], 255};
    SDL_Surface *surfMessage = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfMessage);

    SDL_RenderCopy(renderer, texture, nullptr, dstrect);

    SDL_FreeSurface(surfMessage);
    SDL_DestroyTexture(texture);
}

void App::update()
{
    board->newBlock();
    while (run)
    {
         handlE();
         render();
         if (SDL_GetTicks() % (1000 / complexityLevel) == 0) if(board->fallCheck()) addScore();
#ifndef AUDIO_OFF
         Mix_PlayMusic(musicTheme, -1);
#endif
     }
}

void App::render()
{
    SDL_RenderClear(renderer);
    board->render();
    gameInfoRender();
    SDL_RenderPresent(renderer);
}

void App::addScore()
{
    uint cl = board->checkLines();
    score += cl * cl * 10;
    if(score % 1000 == 0 && score != 0) complexityLevel += 1;
}

void App::handlE()
{
    while (SDL_PollEvent(&event) != 0)
    {
        switch(event.type){
            case SDL_KEYDOWN:
                if(board->keyboardHandle(&event.key)) addScore();
                break;

            case SDL_QUIT:
                run = false;
                break;

            default:
                break;
         }
    }
}

