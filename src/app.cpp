#include "app.h"

App::App(){
    board = nullptr;
    sWnd = nullptr;
    renderer = nullptr;
#ifdef INFO_ON
    font = nullptr;
#endif

#ifndef AUDIO_OFF
    musicTheme = nullptr;
#endif

    if(SDL_Init(SDL_FLAGS) == 0
#ifdef INFO_ON
            && TTF_Init() == 0
#endif
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

#ifndef AUDIO_OFF
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
        musicTheme = Mix_LoadMUS(MAIN_THEME);
#endif

#ifdef INFO_ON
        font = TTF_OpenFont(MAIN_FONT, FONT_PT_SIZE);
#endif
        renderer = SDL_CreateRenderer(sWnd, -1, SDL_RENDERER_ACCELERATED);

        SDL_Surface *icon = IMG_Load("../static/ico/icon.ico");
        SDL_SetWindowIcon(sWnd, icon);

        score = 0;
        recordScore = 0;
        complexityLevel = 1;
        board = new Board(sWnd, renderer);
        run = true;
        delete icon;
    }
}

void App::newGame(){
    if(score > recordScore)
    {
        recordScore = score;
    }
    delete board;
    board = new Board(sWnd, renderer);
    score = 0;
    complexityLevel = 1;
}

#ifdef INFO_ON

static const char *intToChar(uint i, char *buff)
{
    if(i < 10) sprintf(buff, "%d%d%d%d", 0, 0, 0, i);
    else if (i >= 10 && i < 100) sprintf(buff, "%d%d%d", 0, 0, i);
    else if (i >= 100 && i < 1000) sprintf(buff, "%d%d", 0, i);
    else if (i >= 1000 && i < 10000) sprintf(buff, "%d", i);
    return buff;
}

void App::gameInfoRender()
{
    int gmf[4] = GAME_INFO_FRAME;

    char buffer[5];
    const char *scrChar;

    SDL_Rect scl {gmf[0], gmf[1], gmf[2], gmf[3] / 4};
    textRender(" Score: ", &scl);

    SDL_Rect sc {scl.x, scl.y + scl.h, gmf[2], gmf[3] / 4};
    scrChar = intToChar(score, buffer);
    textRender(scrChar, &sc);

    SDL_Rect rscl {scl.x, sc.y + scl.h, gmf[2], gmf[3] / 4};
    textRender(" Record: ", &rscl);

    SDL_Rect rsc {scl.x, rscl.y + scl.h, gmf[2], gmf[3] / 4};
    scrChar = intToChar(recordScore, buffer);
    textRender(scrChar, &rsc);
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

#endif

void App::update()
{
    while (run)
    {
         handlE();
         render();
         if (SDL_GetTicks() % (1000 / complexityLevel) == 0) {
             int check = board->fallCheck();
             if(check == 1) addScore();
             else if (check == -1) newGame();
             else continue;
         }
#ifndef AUDIO_OFF
         Mix_PlayMusic(musicTheme, -1);
#endif
     }
}

void App::render()
{
    SDL_RenderClear(renderer);
    board->render();
#ifdef INFO_ON
    gameInfoRender();
#endif
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
        int check;
        switch(event.type){
            case SDL_KEYDOWN:
                check = board->keyboardHandle(&event.key);
                if(check == 1) addScore();
                else if (check == -1) newGame();
                else continue;
                break;

            case SDL_QUIT:
                run = false;
                break;

            default:
                break;
         }
    }
}

App::~App()
{
    delete board;
#ifndef AUDIO_OFF
    Mix_FreeMusic(musicTheme);
    Mix_Quit();
    musicTheme = nullptr;
#endif

#ifdef INFO_ON
    TTF_CloseFont(font);
    TTF_Quit();
    font = nullptr;
#endif
    SDL_DestroyWindow(sWnd);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    board = nullptr;
    sWnd = nullptr;
    renderer = nullptr;
}

