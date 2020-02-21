#include "app.h"

static Uint32 gameLoop(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}

App::App()
{
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
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0)
        {
            musicTheme = Mix_LoadMUS(MAIN_THEME);
        };
#endif
#ifdef INFO_ON
        font = TTF_OpenFont(MAIN_FONT, FONT_PT_SIZE);
#endif
        renderer = SDL_CreateRenderer(sWnd, -1, SDL_RENDERER_ACCELERATED);

        SDL_Surface *icon = IMG_Load(WIN_ICON);
        SDL_SetWindowIcon(sWnd, icon);
        delete icon;
        readStatistic();
        run = true;
        newGame();
        timerID = SDL_AddTimer(1000 / complexityLevel, gameLoop, nullptr);
    }
}

void App::readStatistic()
{
    std::string line;
    std::string pattern = "Score: ";
    std::ifstream in(STATISTIC_FILE);
    int tmpRecord = 0;
    if (in.is_open())
    {
        while (getline(in, line))
        {
            size_t pos = line.find(pattern);
            if (pos != std::string::npos)
            {
                int score = std::stoi(line.substr(pos + pattern.length()));
                if (tmpRecord < score) tmpRecord = score;
            }
        }
    }
    in.close();
    recordScore = tmpRecord;
}

void App::writeStatistic()
{
    std::ofstream out;
    out.open(STATISTIC_FILE, std::ios::app);
    if (out.is_open())
    {
        time_t seconds = time(NULL);
        tm *timeinfo = localtime(&seconds);
        int playTime = SDL_GetTicks();
        int sec = playTime / 1000;
        int min = sec / 60;
        int h = min / 60;
        out << asctime(timeinfo)
            << ">> Score: " << score << std::endl
            << ">> Level: " << complexityLevel << std::endl
            << ">> Play time: " << h << "h:"
                              << min << "m:"
                              << sec << "s\n" << std::endl;
    }
    out.close();
}

void App::newGame()
{
    writeStatistic();
    if(score > recordScore)
    {
        recordScore = score;
    }
    delete board;
    board = new Board(sWnd, renderer);
    score = 0;
    complexityLevel = 1;
#ifndef AUDIO_OFF
     Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
     Mix_PlayMusic(musicTheme, -1);
#endif
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

void App::gameLoopCheck(int fallFlag)
{
     if(fallFlag == 1) addScore();
     else if (fallFlag == -1) newGame();
}

void App::update()
{
    std::thread keyEventThread(&App::handlE, this);
    while (run)
    {
        render();
    }
    keyEventThread.join();
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
    if (cl > 0)
    {
        score += cl * cl * 10;
        if(score % 1000 == 0 && score != 0 && complexityLevel != 0)
        {
            complexityLevel += 1;
            timerID = SDL_AddTimer(1000 / complexityLevel, gameLoop, nullptr);
        }
    }
}

void App::handlE()
{
    while (run)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            switch(event.type){
                case SDL_KEYDOWN:
                    gameLoopCheck(board->keyboardHandle(&event.key));
                    break;
                case SDL_QUIT:
                    run = false;
                    break;
                case SDL_USEREVENT:
                    gameLoopCheck(board->fallCheck());
                    break;
                default:
                    break;
             }
        }
    }
}

App::~App()
{
    delete board;
    board = nullptr;

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
    sWnd = nullptr;
    renderer = nullptr;
}

