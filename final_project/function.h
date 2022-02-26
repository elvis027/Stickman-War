#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// Project Constant
#define TITLE "Final Project 106060008"
#define WIDTH 800
#define HEIGHT 600
#define BLACK al_map_rgb(0, 0, 0)
#define WHITE al_map_rgb(255, 255, 255)
#define RED al_map_rgb(255, 0, 0)

// Message Constant
typedef enum
{
    // Game Message
    GAME_CONTINUE,
    GAME_TERMINATE,

    // Error Message
    // Initialization Error
    AL_INIT_ERROR,
    AL_INSTALL_AUDIO_ERROR,
    AL_INIT_ACODEC_ADDON_ERROR,
    AL_RESERVE_SAMPLES_ERROR,
    AL_INIT_PRIMITIVES_ADDON_ERROR,
    AL_INSTALL_KEYBOARD_ERROR,
    AL_INIT_IMAGE_ADDON_ERROR,
    AL_INIT_TTF_ADDON_ERROR,
    AL_CREATE_DISPLAY_ERROR,
    AL_CREATE_EVENT_QUEUE_ERROR,
    AL_CREATE_TIMER_ERROR,
    // Load and Play Error
    AL_LOAD_SAMPLE_ERROR,
    AL_PLAY_SAMPLE_ERROR,
    AL_LOAD_TTF_FONT_ERROR,
    AL_LOAD_BITMAP_ERROR,

    // Switch Window Message
    GOTO_MENU,
    GOTO_MAINGAME,
    GOTO_RESULT,
    GOTO_ABOUT
} Message;

// Window Constant
typedef enum
{
    MenuWindow,
    MainGameWindow,
    ResultWindow,
    AboutWindow
} Window;

// Selected Button Constant
typedef enum
{
    StartBtn,
    AboutBtn,
    ExitBtn,
    RestartBtn,
    BackBtn
} SelectedButton;

// Character key Constant
typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ATTACK
} KeyType;

// Button Type
typedef struct
{
    char name[15];
    bool selected;
    float name_x1, name_y1;  // position of text
    float x1_INT, y1_INT, x2_INT, y2_INT;  // internal rectangle
    float x1_EXT, y1_EXT, x2_EXT, y2_EXT;  // external rectangle
} Button;

// Character Type
typedef struct _character
{
    // MainGame Window
    float x, y;
    float HP;
    int dir;  // 0(right), 1(left)
    bool key[5];

    ALLEGRO_BITMAP* Name;
    ALLEGRO_BITMAP* Normal1;
    ALLEGRO_BITMAP* Normal2;
    ALLEGRO_BITMAP* Jump;
    ALLEGRO_BITMAP* Sit;
    ALLEGRO_BITMAP* Walk;
    ALLEGRO_BITMAP* Attack1;
    ALLEGRO_BITMAP* Attack2;
    ALLEGRO_BITMAP* Win;
    ALLEGRO_BITMAP* Lost;
    ALLEGRO_BITMAP* NowState;
    struct _character* Opponent;

    int jumpStep;  // 0(No), 1~5(Jumping)
    ALLEGRO_TIMER* JumpTimer;

    // About Window
    ALLEGRO_BITMAP* AboutPlayer;
    ALLEGRO_BITMAP* Control;
} Character;


void CheckErr(bool done, Message msg);
void GameInit(void);
void GameBegin(void);
Message GameRun(void);
Message ProcessEvent(void);
void GameDestroy(void);
void PlayerDestroy(Character* player);

// Menu
void MenuInit(void);
void MenuDraw(void);
void DrawButton(Button* btn);
void MenuReset(void);

// MainGame
void MainGameInit(void);
void MainGameDraw(void);
void UpdateCharacter(Character* player);
Message JudgeGame(void);
void MainGameReset(void);

// Result
void ResultInit(void);
void ResultDraw(void);
void ResultReset(void);

// About
void AboutInit(void);
void AboutDraw(void);

#endif
