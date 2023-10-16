#include "function.h"

// Environment
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;
// Sample
ALLEGRO_SAMPLE* MenuSong = NULL;
ALLEGRO_SAMPLE* MainGameSong = NULL;
ALLEGRO_SAMPLE* ResultSong = NULL;
// Bitmap
ALLEGRO_BITMAP* MenuBackground = NULL;
ALLEGRO_BITMAP* MainGameBackground = NULL;
ALLEGRO_BITMAP* AboutBackground = NULL;
ALLEGRO_BITMAP* BackgroundMask = NULL;
ALLEGRO_BITMAP* VS = NULL;
// Font
ALLEGRO_FONT* GameTitleFont70 = NULL;
ALLEGRO_FONT* ButtonFont35 = NULL;
ALLEGRO_FONT* ResultFont50 = NULL;
ALLEGRO_FONT* AboutTitleFont30 = NULL;
ALLEGRO_FONT* AboutControlFont20 = NULL;
// Music ID
ALLEGRO_SAMPLE_ID MusicID;

// Menu Button
SelectedButton SelectedBtn;
Button StartButton;
Button AboutButton;
Button ExitButton;
Button RestartButton;
Button BackButton;
// Character
Character Player1;
Character Player2;
// Window
Window window;


// Check Error
void CheckErr(bool done, Message msg)
{
    if(!done) {
        fprintf(stderr, "unexpected msg: %d\n", msg);
        GameDestroy();
        exit(9);
    }
}

// Initialize All Allegro Settings
void GameInit(void)
{
    CheckErr(al_init(), AL_INIT_ERROR);
    CheckErr(al_install_audio(), AL_INSTALL_AUDIO_ERROR);
    CheckErr(al_init_acodec_addon(), AL_INIT_ACODEC_ADDON_ERROR);
    CheckErr(al_reserve_samples(1), AL_RESERVE_SAMPLES_ERROR);
    CheckErr(al_init_primitives_addon(), AL_INIT_PRIMITIVES_ADDON_ERROR);
    CheckErr(al_install_keyboard(), AL_INSTALL_KEYBOARD_ERROR);
    CheckErr(al_init_image_addon(), AL_INIT_IMAGE_ADDON_ERROR);
    al_init_font_addon();
    CheckErr(al_init_ttf_addon(), AL_INIT_TTF_ADDON_ERROR);

    // Create display and event_queue
    CheckErr((display = al_create_display(WIDTH, HEIGHT)), AL_CREATE_DISPLAY_ERROR);
    CheckErr((event_queue = al_create_event_queue()), AL_CREATE_EVENT_QUEUE_ERROR);

    // Initialize display settings
    al_set_window_position(display, 20, 10);
    al_set_window_title(display, TITLE);

    // Register event
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
}

// Initialize All Windows
void GameBegin(void)
{
    MenuInit();
    MainGameInit();
    ResultInit();
    AboutInit();

    window = MenuWindow;
    MenuReset();
    MenuDraw();
}

// Update Game Environment
Message GameRun(void)
{
    Message msg = GAME_CONTINUE;

    if(window == MenuWindow) {
        if(!al_is_event_queue_empty(event_queue)) {
            msg = ProcessEvent();
            MenuDraw();
            if(msg == GOTO_MAINGAME) {
                window = MainGameWindow;
                MainGameReset();
                MainGameDraw();
            }
            else if(msg == GOTO_ABOUT) {
                window = AboutWindow;
                AboutDraw();
            }
        }
    }
    else if(window == MainGameWindow) {
        if(!al_is_event_queue_empty(event_queue)) {
            msg = ProcessEvent();
            MainGameDraw();
            if(msg == GOTO_RESULT) {
                window = ResultWindow;
                ResultReset();
                ResultDraw();
            }
            else if(msg == GOTO_MENU) {
                window = MenuWindow;
                MenuReset();
                MenuDraw();
            }
        }
    }
    else if(window == ResultWindow) {
        if(!al_is_event_queue_empty(event_queue)) {
            msg = ProcessEvent();
            ResultDraw();
            if(msg == GOTO_MAINGAME) {
                window = MainGameWindow;
                MainGameReset();
                MainGameDraw();
            }
            else if(msg == GOTO_MENU) {
                window = MenuWindow;
                MenuReset();
                MenuDraw();
            }
        }
    }
    else if(window == AboutWindow) {
        if(!al_is_event_queue_empty(event_queue)) {
            msg = ProcessEvent();
            if(msg == GOTO_MENU) {
                window = MenuWindow;
                MenuDraw();
            }
        }
    }

    return msg;
}

// Deal with event
Message ProcessEvent(void)
{
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    if(window == MenuWindow) {
        if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    if(SelectedBtn == AboutBtn) {
                        SelectedBtn = StartBtn;
                        AboutButton.selected = false;
                        StartButton.selected = true;
                    }
                    else if(SelectedBtn == ExitBtn) {
                        SelectedBtn = AboutBtn;
                        ExitButton.selected = false;
                        AboutButton.selected = true;
                    }
                    break;
                case ALLEGRO_KEY_DOWN:
                    if(SelectedBtn == StartBtn) {
                        SelectedBtn = AboutBtn;
                        StartButton.selected = false;
                        AboutButton.selected = true;
                    }
                    else if(SelectedBtn == AboutBtn) {
                        SelectedBtn = ExitBtn;
                        AboutButton.selected = false;
                        ExitButton.selected = true;
                    }
                    break;
                case ALLEGRO_KEY_ENTER:
                    if(SelectedBtn == StartBtn)
                        return GOTO_MAINGAME;
                    else if(SelectedBtn == AboutBtn)
                        return GOTO_ABOUT;
                    else if(SelectedBtn == ExitBtn)
                        return GAME_TERMINATE;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    return GAME_TERMINATE;
            }
        }
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return GAME_TERMINATE;
        }
    }
    else if(window == MainGameWindow) {
        if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(event.keyboard.keycode) {
                // Player1 Control
                case ALLEGRO_KEY_W:
                    Player1.key[UP] = true;
                    break;
                case ALLEGRO_KEY_S:
                    Player1.key[DOWN] = true;
                    break;
                case ALLEGRO_KEY_A:
                    Player1.key[LEFT] = true;
                    break;
                case ALLEGRO_KEY_D:
                    Player1.key[RIGHT] = true;
                    break;
                case ALLEGRO_KEY_J:
                    Player1.key[ATTACK] = true;
                    break;
                // Player2 Control
                case ALLEGRO_KEY_UP:
                    Player2.key[UP] = true;
                    break;
                case ALLEGRO_KEY_DOWN:
                    Player2.key[DOWN] = true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    Player2.key[LEFT] = true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    Player2.key[RIGHT] = true;
                    break;
                case ALLEGRO_KEY_PAD_ENTER:
                    Player2.key[ATTACK] = true;
                    break;
                // Return Menu
                case ALLEGRO_KEY_ESCAPE:
                    return GOTO_MENU;
            }
        }
        else if(event.type == ALLEGRO_EVENT_KEY_UP) {
            switch(event.keyboard.keycode) {
                // Player1 Control
                case ALLEGRO_KEY_W:
                    Player1.key[UP] = false;
                    break;
                case ALLEGRO_KEY_S:
                    Player1.key[DOWN] = false;
                    break;
                case ALLEGRO_KEY_A:
                    Player1.key[LEFT] = false;
                    break;
                case ALLEGRO_KEY_D:
                    Player1.key[RIGHT] = false;
                    break;
                case ALLEGRO_KEY_J:
                    Player1.key[ATTACK] = false;
                    break;
                // Player2 Control
                case ALLEGRO_KEY_UP:
                    Player2.key[UP] = false;
                    break;
                case ALLEGRO_KEY_DOWN:
                    Player2.key[DOWN] = false;
                    break;
                case ALLEGRO_KEY_LEFT:
                    Player2.key[LEFT] = false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    Player2.key[RIGHT] = false;
                    break;
                case ALLEGRO_KEY_PAD_ENTER:
                    Player2.key[ATTACK] = false;
                    break;
            }
        }
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return GAME_TERMINATE;
        }

        UpdateCharacter(&Player1);
        UpdateCharacter(&Player2);

        return JudgeGame();
    }
    else if(window == ResultWindow) {
        if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    if(SelectedBtn == BackBtn) {
                        SelectedBtn = RestartBtn;
                        BackButton.selected = false;
                        RestartButton.selected = true;
                    }
                    else if(SelectedBtn == ExitBtn) {
                        SelectedBtn = BackBtn;
                        ExitButton.selected = false;
                        BackButton.selected = true;
                    }
                    break;
                case ALLEGRO_KEY_DOWN:
                    if(SelectedBtn == RestartBtn) {
                        SelectedBtn = BackBtn;
                        RestartButton.selected = false;
                        BackButton.selected = true;
                    }
                    else if(SelectedBtn == BackBtn) {
                        SelectedBtn = ExitBtn;
                        BackButton.selected = false;
                        ExitButton.selected = true;
                    }
                    break;
                case ALLEGRO_KEY_ENTER:
                    if(SelectedBtn == RestartBtn)
                        return GOTO_MAINGAME;
                    else if(SelectedBtn == BackBtn)
                        return GOTO_MENU;
                    else if(SelectedBtn == ExitBtn)
                        return GAME_TERMINATE;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    return GAME_TERMINATE;
            }
        }
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return GAME_TERMINATE;
        }
    }
    else if(window == AboutWindow) {
        if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
            return GOTO_MENU;
        }
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return GAME_TERMINATE;
        }
    }

    return GAME_CONTINUE;
}

// Destroy all the things created
void GameDestroy(void)
{
    // Environment
    if(!display) al_destroy_display(display);
    if(!event_queue) al_destroy_event_queue(event_queue);
    // Sample
    if(!MenuSong) al_destroy_sample(MenuSong);
    if(!MainGameSong) al_destroy_sample(MainGameSong);
    if(!ResultSong) al_destroy_sample(ResultSong);
    // Bitmap
    if(!MenuBackground) al_destroy_bitmap(MenuBackground);
    if(!MainGameBackground) al_destroy_bitmap(MainGameBackground);
    if(!AboutBackground) al_destroy_bitmap(AboutBackground);
    if(!BackgroundMask) al_destroy_bitmap(BackgroundMask);
    if(!VS) al_destroy_bitmap(VS);
    // Font
    if(!GameTitleFont70) al_destroy_font(GameTitleFont70);
    if(!ButtonFont35) al_destroy_font(ButtonFont35);
    if(!ResultFont50) al_destroy_font(ResultFont50);
    if(!AboutTitleFont30) al_destroy_font(AboutTitleFont30);
    if(!AboutControlFont20) al_destroy_font(AboutControlFont20);

    // Character
    PlayerDestroy(&Player1);
    PlayerDestroy(&Player2);
}

void PlayerDestroy(Character* player)
{
    if(!player->Name) al_destroy_bitmap(player->Name);
    if(!player->Normal1) al_destroy_bitmap(player->Normal1);
    if(!player->Normal2) al_destroy_bitmap(player->Normal2);
    if(!player->Jump) al_destroy_bitmap(player->Jump);
    if(!player->Sit) al_destroy_bitmap(player->Sit);
    if(!player->Walk) al_destroy_bitmap(player->Walk);
    if(!player->Attack1) al_destroy_bitmap(player->Attack1);
    if(!player->Attack2) al_destroy_bitmap(player->Attack2);
    if(!player->Win) al_destroy_bitmap(player->Win);
    if(!player->Lost) al_destroy_bitmap(player->Lost);
    if(!player->JumpTimer) al_destroy_timer(player->JumpTimer);
    if(!player->AboutPlayer) al_destroy_bitmap(player->AboutPlayer);
    if(!player->Control) al_destroy_bitmap(player->Control);
}


void MenuInit(void)
{
    // Load the song
    CheckErr((MenuSong = al_load_sample("resource/audio/MenuAudio.wav")), AL_LOAD_SAMPLE_ERROR);

    // Load the playground
    CheckErr((MenuBackground = al_load_bitmap("resource/picture/Menu/MenuBackground.jpg")), AL_LOAD_BITMAP_ERROR);
    CheckErr((BackgroundMask = al_load_bitmap("resource/picture/BackgroundMask.png")), AL_LOAD_BITMAP_ERROR);

    // Load the title font
    CheckErr((GameTitleFont70 = al_load_ttf_font("resource/font/pirulen.ttf", 70, 0)), AL_LOAD_TTF_FONT_ERROR);

    // Load button font and initialize the buttons
    CheckErr((ButtonFont35 = al_load_ttf_font("resource/font/pirulen.ttf", 35, 0)), AL_LOAD_TTF_FONT_ERROR);
    StartButton = (Button) {.name = "Start", .selected = true,
                .name_x1 = WIDTH / 2, .name_y1 = HEIGHT / 2 - 30,
                .x1_INT = 317, .y1_INT = 277, .x2_INT = 483, .y2_INT = 308,
                .x1_EXT = 315, .y1_EXT = 275, .x2_EXT = 485, .y2_EXT = 310};
    AboutButton = (Button) {.name = "About", .selected = false,
                .name_x1 = WIDTH / 2, .name_y1 = HEIGHT / 2 + 40,
                .x1_INT = 312, .y1_INT = 347, .x2_INT = 488, .y2_INT = 378,
                .x1_EXT = 310, .y1_EXT = 345, .x2_EXT = 490, .y2_EXT = 380};
    ExitButton = (Button) {.name = "Exit", .selected = false,
                .name_x1 = WIDTH / 2, .name_y1 = HEIGHT / 2 + 110,
                .x1_INT = 347, .y1_INT = 417, .x2_INT = 453, .y2_INT = 448,
                .x1_EXT = 345, .y1_EXT = 415, .x2_EXT = 455, .y2_EXT = 450};
}

void MenuDraw(void)
{
    // Draw the background
    al_draw_bitmap(MenuBackground, 0, 0, 0);
    al_draw_bitmap(BackgroundMask, 0, 0, 0);

    // Draw the title
    al_draw_text(GameTitleFont70, al_map_rgb(102, 34, 0), WIDTH / 2, HEIGHT / 2 - 150,
                    ALLEGRO_ALIGN_CENTER, "Stickman War");

    // Draw the button
    DrawButton(&StartButton);
    DrawButton(&AboutButton);
    DrawButton(&ExitButton);

    // Show
    al_flip_display();
}

void DrawButton(Button* btn)
{
    ALLEGRO_COLOR color1 = BLACK;
    ALLEGRO_COLOR color2 = WHITE;
    if(btn->selected) {
        color1 = WHITE;
        color2 = BLACK;
    }

    // Draw external rectangle
    al_draw_rectangle(btn->x1_EXT, btn->y1_EXT, btn->x2_EXT, btn->y2_EXT, color1, 3);
    // Draw internal rectangle
    al_draw_filled_rectangle(btn->x1_INT, btn->y1_INT, btn->x2_INT, btn->y2_INT, color2);
    // Draw button name
    al_draw_text(ButtonFont35, color1, btn->name_x1, btn->name_y1, ALLEGRO_ALIGN_CENTER, btn->name);
}

void MenuReset(void)
{
    // Play the song
    al_stop_sample(&MusicID);
    CheckErr(al_play_sample(MenuSong, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &MusicID), AL_PLAY_SAMPLE_ERROR);

    // Select button
    SelectedBtn = StartBtn;
    StartButton.selected = true;
    AboutButton.selected = false;
    ExitButton.selected = false;
}

void MainGameInit(void)
{
    // Load the song
    CheckErr((MainGameSong = al_load_sample("resource/audio/MainGameAudio.wav")), AL_LOAD_SAMPLE_ERROR);

    // Load the playground
    CheckErr((MainGameBackground = al_load_bitmap("resource/picture/MainGame/MainGameBackground.jpg")), AL_LOAD_BITMAP_ERROR);

    // Load the V.S.
    CheckErr((VS = al_load_bitmap("resource/picture/MainGame/VS.png")), AL_LOAD_BITMAP_ERROR);

    // Character initialization
    CheckErr((Player1.Name = al_load_bitmap("resource/picture/Player1Name.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player1.Normal1 = al_load_bitmap("resource/picture/MainGame/Player1Normal1.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player1.Normal2 = al_load_bitmap("resource/picture/MainGame/Player1Normal2.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player1.Jump = al_load_bitmap("resource/picture/MainGame/Player1Jump.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player1.Sit = al_load_bitmap("resource/picture/MainGame/Player1Sit.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player1.Walk = al_load_bitmap("resource/picture/MainGame/Player1Walk.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player1.Attack1 = al_load_bitmap("resource/picture/MainGame/Player1Attack1.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player1.Attack2 = al_load_bitmap("resource/picture/MainGame/Player1Attack2.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player1.Win = al_load_bitmap("resource/picture/MainGame/Player1Win.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player1.Lost = al_load_bitmap("resource/picture/MainGame/Player1Lost.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player1.JumpTimer = al_create_timer(0.05)), AL_CREATE_TIMER_ERROR);
    al_register_event_source(event_queue, al_get_timer_event_source(Player1.JumpTimer));
    Player1.Opponent = &Player2;

    CheckErr((Player2.Name = al_load_bitmap("resource/picture/Player2Name.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player2.Normal1 = al_load_bitmap("resource/picture/MainGame/Player2Normal1.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player2.Normal2 = al_load_bitmap("resource/picture/MainGame/Player2Normal2.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player2.Jump = al_load_bitmap("resource/picture/MainGame/Player2Jump.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player2.Sit = al_load_bitmap("resource/picture/MainGame/Player2Sit.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player2.Walk = al_load_bitmap("resource/picture/MainGame/Player2Walk.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player2.Attack1 = al_load_bitmap("resource/picture/MainGame/Player2Attack1.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player2.Attack2 = al_load_bitmap("resource/picture/MainGame/Player2Attack2.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player2.Win = al_load_bitmap("resource/picture/MainGame/Player2Win.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player2.Lost = al_load_bitmap("resource/picture/MainGame/Player2Lost.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player2.JumpTimer = al_create_timer(0.05)), AL_CREATE_TIMER_ERROR);
    al_register_event_source(event_queue, al_get_timer_event_source(Player2.JumpTimer));
    Player2.Opponent = &Player1;
}

void MainGameDraw(void)
{
    // Draw the background
    al_draw_bitmap(MainGameBackground, 0, 0, 0);

    // Draw the V.S.
    al_draw_bitmap(VS, 365, 15, 0);

    // Draw Character Image
    // Name
    al_draw_bitmap(Player1.Name, 20, 35, 0);
    al_draw_bitmap(Player2.Name, 630, 35, 0);
    // Blood
    al_draw_rectangle(19, 19, 361, 31, BLACK, 2);
    al_draw_filled_rectangle(20, 20, 20 + Player1.HP, 30, RED);
    al_draw_rectangle(439, 19, 781, 31, BLACK, 2);
    al_draw_filled_rectangle(780 - Player2.HP, 20, 780, 30, RED);
    // Character
    al_draw_bitmap(Player1.NowState, Player1.x, Player1.y, Player1.dir);
    al_draw_bitmap(Player2.NowState, Player2.x, Player2.y, Player2.dir);

    // Show
    al_flip_display();
}

void UpdateCharacter(Character* player)
{
    if(!player->jumpStep && !player->key[UP] && !player->key[DOWN] && !player->key[LEFT] && !player->key[RIGHT] && !player->key[ATTACK]) {
        player->y = 250;
        player->NowState = player->Normal1;
    }
    else {

        if(player->key[LEFT]) {
            player->dir = 1;
            if(player->y == 250) player->NowState = player->Walk;
            if(player->x >= 0) player->x -= 5;
            else player->x = WIDTH;
        }
        else if(player->key[RIGHT]) {
            player->dir = 0;
            if(player->y == 250) player->NowState = player->Walk;
            if(player->x <= WIDTH) player->x += 5;
            else player->x = 0;
        }

        if(player->jumpStep) {
            player->NowState = player->Jump;
            if(player->jumpStep == 1) player->y -= 50;
            else if(player->jumpStep == 2) player->y -= 40;
            else if(player->jumpStep == 3) player->y -= 30;
            else if(player->jumpStep == 4) player->y -= 20;
            else if(player->jumpStep == 5) player->y -= 10;
            else if(player->jumpStep == 6) player->y -= 5;
            else if(player->jumpStep == 7);
            else if(player->jumpStep == 8) player->y += 5;
            else if(player->jumpStep == 9) player->y += 10;
            else if(player->jumpStep == 10) player->y += 20;
            else if(player->jumpStep == 11) player->y += 30;
            else if(player->jumpStep == 12) player->y += 40;
            else if(player->jumpStep == 13) player->y += 50;
            player->jumpStep = (player->jumpStep + 1) % 14;
            if(player->jumpStep == 0) {
                al_stop_timer(player->JumpTimer);
                player->NowState = player->Normal1;
            }
        }
        else if(player->key[UP]) {
            player->jumpStep = 1;
            al_start_timer(player->JumpTimer);
        }
        else if(player->key[DOWN]) {
            player->y = 300;
            player->NowState = player->Sit;
        }

        if(player->key[ATTACK]) {
            if(abs(player->x - player->Opponent->x) <= 70 &&
                abs(player->y - player->Opponent->y) <= 20) {
                player->Opponent->HP -= 5;
                if((int)player->Opponent->HP % 2)
                    player->NowState = player->Attack1;
                else
                    player->NowState = player->Attack2;
            }
        }
    }
}

Message JudgeGame(void)
{
    Message msg = GAME_CONTINUE;

    if(Player1.HP == 0) {
        Player1.y += 80;
        Player1.NowState = Player1.Lost;
        Player2.NowState = Player2.Win;
        msg = GOTO_RESULT;
    }
    else if(Player2.HP == 0) {
        Player2.y += 80;
        Player2.NowState = Player2.Lost;
        Player1.NowState = Player1.Win;
        msg = GOTO_RESULT;
    }

    return msg;
}

void MainGameReset(void)
{
    // Play the song
    al_stop_sample(&MusicID);
    CheckErr(al_play_sample(MainGameSong, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &MusicID), AL_PLAY_SAMPLE_ERROR);

    Player1.x = 100;
    Player1.y = 250;
    Player1.HP = 340;
    Player1.dir = 0;
    for(int i = 0; i < 5; i++) Player1.key[i] = false;
    Player1.NowState = Player1.Normal1;
    Player1.jumpStep = 0;

    Player2.x = 600;
    Player2.y = 250;
    Player2.HP = 340;
    Player2.dir = 1;
    for(int i = 0; i < 5; i++) Player2.key[i] = false;
    Player2.NowState = Player2.Normal1;
    Player2.jumpStep = 0;
}

void ResultInit(void)
{
    // Load the song
    CheckErr((ResultSong = al_load_sample("resource/audio/ResultAudio.wav")), AL_LOAD_SAMPLE_ERROR);

    // Load result font
    CheckErr((ResultFont50 = al_load_ttf_font("resource/font/pirulen.ttf", 50, 0)), AL_LOAD_TTF_FONT_ERROR);

    // Initialize the button
    RestartButton = (Button) {.name = "Restart", .selected = true,
                .name_x1 = WIDTH / 2, .name_y1 = HEIGHT / 2 - 30,
                .x1_INT = 287, .y1_INT = 277, .x2_INT = 513, .y2_INT = 308,
                .x1_EXT = 285, .y1_EXT = 275, .x2_EXT = 515, .y2_EXT = 310};
    BackButton = (Button) {.name = "Back To Menu", .selected = false,
                .name_x1 = WIDTH / 2, .name_y1 = HEIGHT / 2 + 40,
                .x1_INT = 212, .y1_INT = 347, .x2_INT = 588, .y2_INT = 378,
                .x1_EXT = 210, .y1_EXT = 345, .x2_EXT = 590, .y2_EXT = 380};
}

void ResultDraw(void)
{
    al_draw_bitmap(BackgroundMask, 0, 0, 0);

    if(Player1.HP == 0) {
        al_draw_text(ResultFont50, WHITE, WIDTH / 2, HEIGHT / 2 - 200,
                        ALLEGRO_ALIGN_CENTER, "Player2 Win!");
    }
    else {
        al_draw_text(ResultFont50, WHITE, WIDTH / 2, HEIGHT / 2 - 200,
                        ALLEGRO_ALIGN_CENTER, "Player1 Win!");
    }

    // Draw the button
    DrawButton(&RestartButton);
    DrawButton(&BackButton);
    DrawButton(&ExitButton);

    al_flip_display();
}

void ResultReset(void)
{
    // Play the song
    al_stop_sample(&MusicID);
    CheckErr(al_play_sample(ResultSong, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &MusicID), AL_PLAY_SAMPLE_ERROR);

    // Select button
    SelectedBtn = RestartBtn;
    RestartButton.selected = true;
    BackButton.selected = false;
    ExitButton.selected = false;
}

void AboutInit(void)
{
    // Load the background
    CheckErr((AboutBackground = al_load_bitmap("resource/picture/About/AboutBackground.jpg")), AL_LOAD_BITMAP_ERROR);

    // Load the title font
    CheckErr((AboutTitleFont30 = al_load_ttf_font("resource/font/pirulen.ttf", 30, 0)), AL_LOAD_TTF_FONT_ERROR);

    // Load the control font
    CheckErr((AboutControlFont20 = al_load_ttf_font("resource/font/pirulen.ttf", 20, 0)), AL_LOAD_TTF_FONT_ERROR);

    // Load About part of Character
    CheckErr((Player1.AboutPlayer = al_load_bitmap("resource/picture/About/AboutPlayer1.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player1.Control = al_load_bitmap("resource/picture/About/Player1Control.png")), AL_LOAD_BITMAP_ERROR);

    CheckErr((Player2.AboutPlayer = al_load_bitmap("resource/picture/About/AboutPlayer2.png")), AL_LOAD_BITMAP_ERROR);
    CheckErr((Player2.Control = al_load_bitmap("resource/picture/About/Player2Control.png")), AL_LOAD_BITMAP_ERROR);
}

void AboutDraw(void)
{
    // Draw the background
    al_draw_bitmap(AboutBackground, 0, 0, 0);

    // Draw the title
    al_draw_text(AboutTitleFont30, al_map_rgb(77, 0, 0), 400, 25,
                    ALLEGRO_ALIGN_CENTER, "About the game");

    // Draw the control text
    al_draw_text(AboutControlFont20, BLACK, 260, 300,
                    ALLEGRO_ALIGN_CENTER, "W:UP");
    al_draw_text(AboutControlFont20, BLACK, 260, 350,
                    ALLEGRO_ALIGN_CENTER, "S:DOWN");
    al_draw_text(AboutControlFont20, BLACK, 260, 400,
                    ALLEGRO_ALIGN_CENTER, "A:LEFT");
    al_draw_text(AboutControlFont20, BLACK, 260, 450,
                    ALLEGRO_ALIGN_CENTER, "D:RIGHT");
    al_draw_text(AboutControlFont20, BLACK, 260, 520,
                    ALLEGRO_ALIGN_CENTER, "J:ATTACK");

    al_draw_text(AboutControlFont20, BLACK, 500, 300,
                    ALLEGRO_ALIGN_CENTER, "UP:UP");
    al_draw_text(AboutControlFont20, BLACK, 500, 350,
                    ALLEGRO_ALIGN_CENTER, "DOWN:DOWN");
    al_draw_text(AboutControlFont20, BLACK, 500, 400,
                    ALLEGRO_ALIGN_CENTER, "LEFT:LEFT");
    al_draw_text(AboutControlFont20, BLACK, 500, 450,
                    ALLEGRO_ALIGN_CENTER, "RIGHT:RIGHT");
    al_draw_text(AboutControlFont20, BLACK, 500, 520,
                    ALLEGRO_ALIGN_CENTER, "ENTER:ATTACK");

    // Draw the character image
    // Name
    al_draw_bitmap(Player1.Name, 20, 90, 0);
    al_draw_bitmap(Player2.Name, 630, 90, 0);
    // Control
    al_draw_bitmap(Player1.Control, 170, 140, 0);
    al_draw_bitmap(Player2.Control, 400, 140, 0);
    // Player
    al_draw_bitmap(Player1.AboutPlayer, 0, 150, 0);
    al_draw_bitmap(Player2.AboutPlayer, 600, 150, 1);

    // Show
    al_flip_display();
}
