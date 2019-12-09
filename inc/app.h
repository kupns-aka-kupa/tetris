#pragma once
#include <SDL.h>
#include <SDL_image.h>

#ifndef AUDIO_OFF
#include <SDL_mixer.h>
#endif

#include <SDL_ttf.h>
#include "board.h"

class App
{
    uint score;
    uint complexityLevel;
    bool run;
    SDL_Window *sWnd;
    SDL_Renderer *renderer;
    SDL_Event event;
    TTF_Font *font;
    Board *board;
#ifndef AUDIO_OFF
    Mix_Music *musicTheme;
#endif


public:
    App(){
        board = nullptr;
        sWnd = nullptr;
        renderer = nullptr;
        font = nullptr;
#ifndef AUDIO_OFF
        musicTheme = nullptr;
#endif
        init();
    }

    ~App()
    {
        delete board;
#ifndef AUDIO_OFF
        Mix_FreeMusic(musicTheme);
        Mix_Quit();
        musicTheme = nullptr;
#endif
        TTF_CloseFont(font);
        SDL_DestroyWindow(sWnd);
        SDL_DestroyRenderer(renderer);
        TTF_Quit();
        SDL_Quit();
        board = nullptr;
        sWnd = nullptr;
        renderer = nullptr;
        font = nullptr;
    }

    bool init();
    void textRender(const char *text, SDL_Rect *dstrect);
    void gameInfoRender();
    void render();
    void addScore();
    void handlE();
    void update();

};
