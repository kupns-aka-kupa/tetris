#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include <string>
#include <thread>

#include "board.h"

#ifndef AUDIO_OFF
    #include <SDL_mixer.h>
#endif

#ifdef INFO_ON
    #include <SDL_ttf.h>
#endif

class App
{
    uint score;
    uint recordScore;
    uint complexityLevel;
    bool run;
    SDL_Window *sWnd;
    SDL_Renderer *renderer;
    SDL_Event event;
    SDL_TimerID timerID;
    Board *board;

#ifdef INFO_ON
    TTF_Font *font;
#endif

#ifndef AUDIO_OFF
    Mix_Music *musicTheme;
#endif

public:
    App();
    ~App();

#ifdef INFO_ON
    void textRender(const char *text, SDL_Rect *dstrect);
    void gameInfoRender();
#endif
    void render();

    void newGame();
    void gameLoopCheck(int fallFlag);
    void update();
    void addScore();

    void handlE();

    void writeStatistic();
    void readStatistic();


};
