#ifndef CONF_h_

#define CONF_h_

/* Constant configuration, must not be changed */

#define AUDIO_OFF
#define PROJECT_NAME "Tetris"
#define Tetris_VERSION_MAJOR 1
#define Tetris_VERSION_MINOR 0

#define SDL_BASIC_FLAGS (SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER)

#ifndef AUDIO_OFF

    #define MAIN_THEME "/root/cpp/tetris/static/music/maintheme.wav"
    #define SDL_FLAGS (SDL_BASIC_FLAGS | SDL_INIT_AUDIO)

#else

    #define SDL_FLAGS SDL_BASIC_FLAGS

#endif

#define MAIN_FONT "/root/cpp/tetris/static/fonts/mainfont.ttf"
#define SWND_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS

/*
 *  Configure value for tetris,
 *      which can be changed before compilation
 *      and affect the size of the window, of the blocks
 *      and the Tetris itself.
 *
 *  BLC_SCL :
 *      description : block scale in px,
 *      standart : 20,
 *      recomended : [5 - 60];
 *
 *  RC :
 *      description : row count,
 *      standart : 15,
 *      minimum : > 0,
 *      recomended : [12 - 20];
 *
 *  CC :
 *      description : collumn count,
 *      standart : 17,
 *      minimum : > 0,
 *      recomended : [15 - 25];
 */

#define BLC_SCL 20
#define RC 17
#define CC 11

// Сonfiguration-dependent values that should not be changed

#define FO (BLC_SCL / 2) // frame offset

#define TETRIS_WIDTH (CC * BLC_SCL)
#define TETRIS_HEIGHT (RC * BLC_SCL)

#define INFO_ON
/*
 * Enable info panel
 * Can be change in CmakeLists.txt: set(INFO {ON -> OFF, OFF -> ON})
 * INFO_ON || INFO_OFF
 *         ||
 *         \/
 */
#ifdef INFO_ON
    #define FONT_PT_SIZE (BLC_SCL * 10)

    #define GAME_INFO_PANEL_WIDTH (TETRIS_WIDTH / 2)
    #define GAME_INFO_PANEL_HEIGHT (TETRIS_HEIGHT * 2 / 3)

    #define BLOCK_IFNO_PANEL_WIDTH GAME_INFO_PANEL_WIDTH
    #define BLOCK_IFNO_PANEL_HEIGHT (TETRIS_HEIGHT - GAME_INFO_PANEL_HEIGHT)

    #define SCREEN_WIDTH (TETRIS_WIDTH + GAME_INFO_PANEL_WIDTH + 3 * FO)
    #define SCREEN_HEIGHT (TETRIS_HEIGHT + 2 * FO)

    #define INFO_SWAP_OFF
    /*
     * Swap info panels mutually
     * Can be change in CmakeLists.txt: set(INFO_SWAP {ON -> OFF, OFF -> ON})
     * INFO_SWAP_ON | INFO_SWAP_OFF
     * ___________________
     * | ______ | ______ |
     * | |blok| | |game| |
     * | |____| | |info| |
     * | ______ | |    | |
     * | |game| | |____| |
     * | |info| | ______ |
     * | |    | | |blok| |
     * | |____| | |____| |
     * |________|________|
     *
     */
    #ifdef INFO_SWAP_OFF

        #define GAME_INFO_FRAME {TETRIS_WIDTH + 2 * FO, FO, GAME_INFO_PANEL_WIDTH, GAME_INFO_PANEL_HEIGHT - FO}
        #define BLOCK_INFO_FRAME {TETRIS_WIDTH + 2 * FO, GAME_INFO_PANEL_HEIGHT + FO, BLOCK_IFNO_PANEL_WIDTH, BLOCK_IFNO_PANEL_HEIGHT}

    #else

        #define GAME_INFO_FRAME {TETRIS_WIDTH + 2 * FO, BLOCK_IFNO_PANEL_HEIGHT + 2 * FO, GAME_INFO_PANEL_WIDTH, GAME_INFO_PANEL_HEIGHT - FO}
        #define BLOCK_INFO_FRAME {TETRIS_WIDTH + 2 * FO, FO, BLOCK_IFNO_PANEL_WIDTH, BLOCK_IFNO_PANEL_HEIGHT}

    #endif
#else

    #define SCREEN_WIDTH (TETRIS_WIDTH + 2 * FO)
    #define SCREEN_HEIGHT (TETRIS_HEIGHT + 2 * FO)

#endif

#define MAIN_FRAME {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}
#define TETRIS_FRAME {FO, FO, TETRIS_WIDTH, TETRIS_HEIGHT}

#define DEFAULT_TETRIS_MODE_OFF

#endif //CONF_h_
