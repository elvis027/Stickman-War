#include <stdio.h>
#include<time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define GAME_TERMINATE -1

//P1 control
#define W 0
#define S 1
#define A 2
#define D 3
#define J 4

//P2 control
#define UP 5
#define DOWN 6
#define LEFT 7
#define RIGHT 8
#define PAD_1 9

// ALLEGRO Variables
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_BITMAP *background1 = NULL;
ALLEGRO_BITMAP *background2 = NULL;
ALLEGRO_BITMAP *background3 = NULL;
ALLEGRO_BITMAP *background4 = NULL;
//ALLEGRO_TIMER *timer1 = NULL;
//ALLEGRO_TIMER *timer2 = NULL;
ALLEGRO_SAMPLE *song1=NULL;
ALLEGRO_SAMPLE *song2=NULL;
ALLEGRO_SAMPLE *song3=NULL;
ALLEGRO_FONT *font = NULL;

//Custom Definition
const char *title = "Final Project 106060008";
const float FPS = 60;
const int WIDTH = 800;
const int HEIGHT = 600;
typedef struct character
{
    int x;
    int y;
    ALLEGRO_BITMAP *image_path;

}Character;

Character Player1;
//character Player1S;
Character Player2;
//character Player2S;

bool G_begin = false;
int window = 1;
bool judge_window2 = false;
bool judge_window3 = false;
bool ture_1 , ture_2;

ALLEGRO_SAMPLE_ID musicid;
int menu1_x, menu2_x;
int dirP1=RIGHT, dirP2=LEFT;
int x1,x2;
bool key[20];
int HP1=340,HP2=340;
int AtP1=0, AtP2=0;

void show_err_msg(int msg);
void game_init();
void game_begin();
void reset();
int process_event();
int game_run();
void game_destroy();
void Delay(int secs);

int main(int argc, char *argv[]){
    int msg = 0;

    game_init();

    while (msg != GAME_TERMINATE) {
        msg = game_run();
        if (msg == GAME_TERMINATE)
            printf("Game Over\n");
    }

    game_destroy();
    return 0;
}

void show_err_msg(int msg){
    fprintf(stderr, "unexpected msg: %d\n", msg);
    game_destroy();
    exit(9);
}

void game_init(){
    if (!al_init()) {
        show_err_msg(-1);
    }
    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        show_err_msg(-1);
    }
    if(!al_init_acodec_addon()){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        show_err_msg(-1);
    }
    if (!al_reserve_samples(1)){
        fprintf(stderr, "failed to reserve samples!\n");
        show_err_msg(-1);
    }
    // Create display
    display = al_create_display(WIDTH, HEIGHT);
    event_queue = al_create_event_queue();
    if (display == NULL || event_queue == NULL) {
        show_err_msg(-1);
    }
    // Initialize Allegro settings
    al_set_window_position(display, 0, 0);
    al_set_window_title(display, title);
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    // Register event
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
}

void game_begin(){
    // Load sound
    song1 = al_load_sample( "BGM1.wav" );
    song2 = al_load_sample( "BGM2.wav" );
    song3 = al_load_sample( "BGM3.wav" );

    if (!song1 || !song2 || !song3){
        printf( "Audio clip sample not loaded!\n" );
        show_err_msg(-1);
    }

    // Loop the song until the display closes
    al_play_sample(song1, 1.0, 0.0, 1.0,ALLEGRO_PLAYMODE_LOOP,&musicid);

    // Load backgroud
    background1 = al_load_bitmap("background1.jpg");
    if (background1 == NULL)
        show_err_msg(-1);
    al_draw_bitmap(background1, 0, 0, 0);
    al_draw_bitmap(al_load_bitmap("background3.png"), 0, 0, 0);

    // Load and draw text
    al_draw_filled_rectangle(317, 277, 483, 308, al_map_rgb(0,0,0));
    menu1_x = 317;
    al_draw_filled_rectangle(312, 347, 488, 378, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(347, 417, 453, 448, al_map_rgb(255, 255, 255));
    al_draw_rectangle(315, 275, 485, 310, al_map_rgb(255, 255, 255), 3);
    al_draw_rectangle(310, 345, 490, 380, al_map_rgb(0,0,0), 3);
    al_draw_rectangle(345, 415, 455, 450, al_map_rgb(0,0,0), 3);

    font = al_load_ttf_font("pirulen.ttf",35,0);
    al_draw_text(al_load_ttf_font("pirulen.ttf",70,0), al_map_rgb(102,34,0),
                 WIDTH/2, HEIGHT/2-150 , ALLEGRO_ALIGN_CENTRE, "Stickman War");

    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2-30 ,
                 ALLEGRO_ALIGN_CENTRE, "Start");
    al_draw_text(font, al_map_rgb(0,0,0), WIDTH/2, HEIGHT/2+40 ,
                 ALLEGRO_ALIGN_CENTRE, "About");
    al_draw_text(font, al_map_rgb(0,0,0), WIDTH/2, HEIGHT/2+110 ,
                 ALLEGRO_ALIGN_CENTRE, "Exit");
    al_flip_display();
}

int process_event(){
    // Request the event
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    // Our setting for controlling animation
    /*if(event.timer.source == timer1){
        ture_1 = !ture_1 ;
    }
    if(event.timer.source == timer2){
        ture_2 = !ture_2 ;
    }*/

    // Keyboard
    if(event.type == ALLEGRO_EVENT_KEY_DOWN){
        if(window==1){ //Menu1
            switch(event.keyboard.keycode)
            {
            // Menu Up and Down
            case ALLEGRO_KEY_UP:
                if(menu1_x == 312){ //About->Start
                    al_draw_filled_rectangle(312, 347, 488, 378, al_map_rgb(255,255,255));
                    al_draw_rectangle(310, 345, 490, 380, al_map_rgb(0,0,0), 3);
                    al_draw_text(font, al_map_rgb(0,0,0), WIDTH/2, HEIGHT/2+40 , ALLEGRO_ALIGN_CENTRE, "About");

                    al_draw_filled_rectangle(317, 277, 483, 308, al_map_rgb(0,0,0));
                    al_draw_rectangle(315, 275, 485, 310, al_map_rgb(255,255,255), 3);
                    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2-30 , ALLEGRO_ALIGN_CENTRE, "Start");

                    menu1_x = 317;
                }else if(menu1_x == 347){ //Exit->About
                    al_draw_filled_rectangle(347, 417, 453, 448, al_map_rgb(255,255,255));
                    al_draw_rectangle(345, 415, 455, 450, al_map_rgb(0,0,0), 3);
                    al_draw_text(font, al_map_rgb(0,0,0), WIDTH/2, HEIGHT/2+110 , ALLEGRO_ALIGN_CENTRE, "Exit");

                    al_draw_filled_rectangle(312, 347, 488, 378, al_map_rgb(0,0,0));
                    al_draw_rectangle(310, 345, 490, 380, al_map_rgb(255,255,255), 3);
                    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+40 , ALLEGRO_ALIGN_CENTRE, "About");

                    menu1_x = 312;
                }
                al_flip_display();
                break;
            case ALLEGRO_KEY_DOWN:
                if(menu1_x == 317){ //Start->About
                    al_draw_filled_rectangle(317, 277, 483, 308, al_map_rgb(255,255,255));
                    al_draw_rectangle(315, 275, 485, 310, al_map_rgb(0,0,0), 3);
                    al_draw_text(font, al_map_rgb(0,0,0), WIDTH/2, HEIGHT/2-30 , ALLEGRO_ALIGN_CENTRE, "Start");

                    al_draw_filled_rectangle(312, 347, 488, 378, al_map_rgb(0,0,0));
                    al_draw_rectangle(310, 345, 490, 380, al_map_rgb(255,255,255), 3);
                    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+40 , ALLEGRO_ALIGN_CENTRE, "About");

                    menu1_x = 312;
                }else if(menu1_x == 312){ //About->Exit
                    al_draw_filled_rectangle(312, 347, 488, 378, al_map_rgb(255,255,255));
                    al_draw_rectangle(310, 345, 490, 380, al_map_rgb(0,0,0), 3);
                    al_draw_text(font, al_map_rgb(0,0,0), WIDTH/2, HEIGHT/2+40 , ALLEGRO_ALIGN_CENTRE, "About");

                    al_draw_filled_rectangle(347, 417, 453, 448, al_map_rgb(0,0,0));
                    al_draw_rectangle(345, 415, 455, 450, al_map_rgb(255,255,255), 3);
                    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+110 , ALLEGRO_ALIGN_CENTRE, "Exit");

                    menu1_x = 347;
                }
                al_flip_display();
                break;
            case ALLEGRO_KEY_ENTER:
                if(menu1_x == 317){
                    judge_window2 = true;
                }else if(menu1_x == 312){
                    judge_window3 = true;
                }else if(menu1_x == 347){
                    return GAME_TERMINATE;
                }
                break;
            }
        }
        if(window==2 && (HP1 != 0 && HP2 != 0)){ //Key true
            switch(event.keyboard.keycode)
            {
            // P1 control
            case ALLEGRO_KEY_W:
                key[W] = true;
                break;
            case ALLEGRO_KEY_S:
                key[S] = true;
                break;
            case ALLEGRO_KEY_A:
                key[A] = true;
                break;
            case ALLEGRO_KEY_D:
                key[D] = true;
                break;
            // P1 Attack
            case ALLEGRO_KEY_J:
                key[J] = true;
                break;


            // P2 control
            case ALLEGRO_KEY_UP:
                key[UP] = true;
                break;
            case ALLEGRO_KEY_DOWN:
                key[DOWN] = true;
                break;
            case ALLEGRO_KEY_LEFT:
                key[LEFT] = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                key[RIGHT] = true;
                break;
            // P2 Attack
            case ALLEGRO_KEY_PAD_1:
                key[PAD_1] = true;
            }
        }
        if(window==2 && (HP1 <= 0 || HP2 <= 0)){ //Menu2
            switch(event.keyboard.keycode)
            {
            // Menu Up and Down
            case ALLEGRO_KEY_UP:
                if(menu2_x == 212){ //Back to menu->Restart
                    al_draw_filled_rectangle(212, 347, 588, 378, al_map_rgb(255,255,255));
                    al_draw_rectangle(210, 345, 590, 380, al_map_rgb(0,0,0), 3);
                    al_draw_text(font, al_map_rgb(0,0,0), WIDTH/2, HEIGHT/2+40 , ALLEGRO_ALIGN_CENTRE, "Back to menu");

                    al_draw_filled_rectangle(287, 277, 513, 308, al_map_rgb(0,0,0));
                    al_draw_rectangle(285, 275, 515, 310, al_map_rgb(255,255,255), 3);
                    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2-30 , ALLEGRO_ALIGN_CENTRE, "Restart");

                    menu2_x = 287;
                }else if(menu2_x == 347){ //Exit->Back to menu
                    al_draw_filled_rectangle(347, 417, 453, 448, al_map_rgb(255,255,255));
                    al_draw_rectangle(345, 415, 455, 450, al_map_rgb(0,0,0), 3);
                    al_draw_text(font, al_map_rgb(0,0,0), WIDTH/2, HEIGHT/2+110 , ALLEGRO_ALIGN_CENTRE, "Exit");

                    al_draw_filled_rectangle(212, 347, 588, 378, al_map_rgb(0,0,0));
                    al_draw_rectangle(210, 345, 590, 380, al_map_rgb(255,255,255), 3);
                    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+40 , ALLEGRO_ALIGN_CENTRE, "Back to menu");

                    menu2_x = 212;
                }
                break;
            case ALLEGRO_KEY_DOWN:
                if(menu2_x == 287){ //Restart->Back to menu
                    al_draw_filled_rectangle(287, 277, 513, 308, al_map_rgb(255,255,255));
                    al_draw_rectangle(285, 275, 515, 310, al_map_rgb(0,0,0), 3);
                    al_draw_text(font, al_map_rgb(0,0,0), WIDTH/2, HEIGHT/2-30 , ALLEGRO_ALIGN_CENTRE, "Restart");

                    al_draw_filled_rectangle(212, 347, 588, 378, al_map_rgb(0,0,0));
                    al_draw_rectangle(210, 345, 590, 380, al_map_rgb(255,255,255), 3);
                    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+40 , ALLEGRO_ALIGN_CENTRE, "Back to menu");

                    menu2_x = 212;
                }else if(menu2_x == 212){ //Back to menu->Exit
                    al_draw_filled_rectangle(212, 347, 588, 378, al_map_rgb(255,255,255));
                    al_draw_rectangle(210, 345, 590, 380, al_map_rgb(0,0,0), 3);
                    al_draw_text(font, al_map_rgb(0,0,0), WIDTH/2, HEIGHT/2+40 , ALLEGRO_ALIGN_CENTRE, "Back to menu");

                    al_draw_filled_rectangle(347, 417, 453, 448, al_map_rgb(0,0,0));
                    al_draw_rectangle(345, 415, 455, 450, al_map_rgb(255,255,255), 3);
                    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+110 , ALLEGRO_ALIGN_CENTRE, "Exit");

                    menu2_x = 347;
                }
                break;
            case ALLEGRO_KEY_ENTER:
                if(menu2_x == 287){
                    reset();
                    G_begin = true;
                    judge_window2 = true;
                }else if(menu2_x == 212){
                    reset();
                }else if(menu2_x == 347){
                    return GAME_TERMINATE;
                }
                break;
            }
        }
        if(window==3){ //About
            if(event.keyboard.keycode == ALLEGRO_KEY_B)
               reset();
               return 0;
        }
    }
    else if(event.type == ALLEGRO_EVENT_KEY_UP){
        if(window==2){ //Key false;
            switch(event.keyboard.keycode)
            {
            // P1 control
            case ALLEGRO_KEY_W:
                key[W] = false;
                break;
            case ALLEGRO_KEY_S:
                key[S] = false;
                break;
            case ALLEGRO_KEY_A:
                key[A] = false;
                break;
            case ALLEGRO_KEY_D:
                key[D] = false;
                break;
            // P1 Attack
            case ALLEGRO_KEY_J:
                key[J] = false;
                break;

            // P2 control
            case ALLEGRO_KEY_UP:
                key[UP] = false;
                break;
            case ALLEGRO_KEY_DOWN:
                key[DOWN] = false;
                break;
            case ALLEGRO_KEY_LEFT:
                key[LEFT] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                key[RIGHT] = false;
                break;
            // P2 Attack
            case ALLEGRO_KEY_PAD_1:
                key[PAD_1] = false;
            }
        }
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){ // Shutdown our program
        return GAME_TERMINATE;
    }

    if(window == 2 && (HP1 !=0 && HP2 != 0)){ //Judge key
        if(key[W]){
            if(Player1.y > 80) Player1.y -= 10;
            Player1.image_path = al_load_bitmap("Player1J.png");
        }
        if(key[S]){
            if(Player1.y + 30 <= 250) Player1.y += 30;
            else Player1.y = 250;
            if(Player1.y == 250){
                Player1.y += 50;
                Player1.image_path = al_load_bitmap("Player1D.png");
            }
        }
        if(key[A]){
            dirP1 = LEFT;
            if(Player1.y == 250) Player1.image_path = al_load_bitmap("Player1W.png");
            if(Player1.x >= 5 ) Player1.x -= 5;
            else Player1.x = WIDTH;
        }
        if(key[D]){
            dirP1 = RIGHT;
            if(Player1.y == 250) Player1.image_path = al_load_bitmap("Player1W.png");
            if(Player1.x <= WIDTH - 5 )Player1.x += 5;
            else Player1.x = 0;
        }
        if(key[J]){
            if(AtP1<2){
                Player1.image_path = al_load_bitmap("Player1A1.png");
                AtP1++;
            }else if(AtP1<4){
                Player1.image_path = al_load_bitmap("Player1A2.png");
                if((Player1.x - Player2.x <= 70) && (Player1.x - Player2.x >= -70)
                   && (Player1.y - Player2.y <= 20) && (Player1.y - Player2.y >= -20)){
                    if(HP2>=0) HP2--;
                }
                AtP1++;
            }else AtP1 = 0;
        }else if(!key[J] && !key[W] && !key[S] && !key[A] && !key[D]){
            if(Player1.y + 20 <= 250) Player1.y += 20;
                else Player1.y = 250;
            if(Player1.y == 250)
                Player1.image_path = al_load_bitmap("Player1.png");
        }

        if(key[UP]){
            if(Player2.y > 80) Player2.y -= 10;
            Player2.image_path = al_load_bitmap("Player2J.png");
        }
        if(key[DOWN]){
            if(Player2.y + 30 <= 250) Player2.y += 30;
            else Player2.y = 250;
            if(Player2.y == 250){
                Player2.y += 50;
                Player2.image_path = al_load_bitmap("Player2D.png");
            }
        }
        if(key[LEFT]){
            dirP2 = LEFT;
            if(Player2.y == 250) Player2.image_path = al_load_bitmap("Player2W.png");
            if(Player2.x >= 5 ) Player2.x -= 5;
            else Player2.x = WIDTH;
        }
        if(key[RIGHT]){
            dirP2 = RIGHT;
            if(Player2.y == 250) Player2.image_path = al_load_bitmap("Player2W.png");
            if(Player2.x <= WIDTH - 5 )Player2.x += 5;
            else Player2.x = 0;
        }
        if(key[PAD_1]){
            if(AtP2<2){
                Player2.image_path = al_load_bitmap("Player2A1.png");
                AtP2++;
            }else if(AtP2<4){
                Player2.image_path = al_load_bitmap("Player2A2.png");
                if((Player1.x - Player2.x <= 70) && (Player1.x - Player2.x >= -70)
                   && (Player1.y - Player2.y <= 20) && (Player1.y - Player2.y >= -20)){
                    if(HP1>=0)HP1--;
                }
                AtP2++;
            }else AtP2 = 0;
        }else if(!key[PAD_1] && !key[UP] && !key[DOWN] && !key[LEFT] && !key[RIGHT]){
            if(Player2.y + 20 <= 250) Player2.y += 20;
                else Player2.y = 250;
            if(Player2.y == 250)
                Player2.image_path = al_load_bitmap("Player2.png");
        }

        if(HP1 == 0){ //Judge result
            Player2.image_path = al_load_bitmap("Player2Win.png");
            Player1.y += 80;
            Player1.image_path = al_load_bitmap("Player1Lost.png");
        }
        else if(HP2 == 0){
            Player1.image_path = al_load_bitmap("Player1Win.png");
            Player2.y += 80;
            Player2.image_path = al_load_bitmap("Player2Lost.png");
        }
    }

    return 0;
}

int game_run(){
    int error = 0;

    // First window(Menu)
    if(window == 1){
        if(!G_begin){
            game_begin();
            G_begin = true;
        }
        if (!al_is_event_queue_empty(event_queue)) {
            error = process_event();
            if(judge_window2){
                window = 2;
                //Change song
                al_stop_sample(&musicid);
                al_play_sample(song2, 1.0, 0.0, 1.0,ALLEGRO_PLAYMODE_LOOP,&musicid);
                // Setting Character
                Player1.x = 100;
                Player1.y = 250;
                Player2.x = 600;
                Player2.y = 250;
                Player1.image_path = al_load_bitmap("Player1.png");
                //Player1S.image_path = al_load_bitmap("Player1S.png");
                Player2.image_path = al_load_bitmap("player2.png");
                //Player2S.image_path = al_load_bitmap("player2S.png");
                background2 = al_load_bitmap("background2.jpg");
                background3 = al_load_bitmap("Background3.png");

                //Initialize Timer
                /*timer1  = al_create_timer(1.0/3.0);
                timer2  = al_create_timer(1.0/3.0);
                al_register_event_source(event_queue, al_get_timer_event_source(timer1)) ;
                al_register_event_source(event_queue, al_get_timer_event_source(timer2)) ;
                al_start_timer(timer1);
                al_start_timer(timer2);*/
            }
            else if(judge_window3){
                window = 3;

                Player1.x = 0;
                Player1.y = 150;
                Player2.x = 600;
                Player2.y = 150;
                Player1.image_path = al_load_bitmap("ABPlayer1.png");
                Player2.image_path = al_load_bitmap("ABPlayer2.png");

                background4 = al_load_bitmap("background4.jpg");
            }
        }
    }

    // Second window(Main Game)
    else if(window == 2){
        if(HP1 >= 0 && HP2 >= 0){
            // Change Image for animation
            al_draw_bitmap(background2, 0, 0, 0);

            //Blood
            al_draw_bitmap(al_load_bitmap("vs.png"), 365, 15, 0);
            al_draw_bitmap(al_load_bitmap("Player1N.png"), 20, 35, 0);
            al_draw_bitmap(al_load_bitmap("Player2N.png"), 630, 35, 0);
            al_draw_filled_rectangle(20     , 20, 20+HP1 , 30, al_map_rgb(255, 0, 0));
            al_draw_filled_rectangle(780-HP2, 20, 780    , 30, al_map_rgb(255, 0, 0));
            al_draw_rectangle(19 , 19, 361, 31, al_map_rgb(0, 0, 0), 2);
            al_draw_rectangle(439, 19, 781, 31, al_map_rgb(0, 0, 0), 2);

            if(dirP1 == RIGHT) x1 = 0; else x1 = 1;
            if(dirP2 == RIGHT) x2 = 0; else x2 = 1;
            al_draw_bitmap(Player1.image_path, Player1.x, Player1.y, x1);
            al_draw_bitmap(Player2.image_path, Player2.x, Player2.y, x2);

            /*if(ture_1)al_draw_bitmap(Player1.image_path, Player1.x, Player1.y, x1);
            else al_draw_bitmap(Player1S.image_path, Player1.x, Player1.y, x1);

            if(ture_2)al_draw_bitmap(Player2.image_path, Player2.x, Player2.y, x2);
            else al_draw_bitmap(Player2S.image_path, Player2.x, Player2.y, x2);*/

            // Judge game over
            if(HP1 == 0 || HP2 == 0) {
                //Change song
                al_stop_sample(&musicid);
                al_play_sample(song3, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,&musicid);
                al_draw_bitmap(background3, 0, 0, 0);

                if(HP1 == 0)
                    al_draw_text(al_load_ttf_font("pirulen.ttf",50,0), al_map_rgb(255,255,255),
                                 WIDTH/2, HEIGHT/2-200 , ALLEGRO_ALIGN_CENTRE, "Player2 Win!");
                else
                    al_draw_text(al_load_ttf_font("pirulen.ttf",50,0), al_map_rgb(255,255,255),
                                 WIDTH/2, HEIGHT/2-200 , ALLEGRO_ALIGN_CENTRE, "Player1 Win!");
                al_flip_display();
                Delay(2);

                al_draw_filled_rectangle(287, 277, 513, 308, al_map_rgb(0,0,0));
                menu2_x = 287;
                al_draw_filled_rectangle(212, 347, 588, 378, al_map_rgb(255, 255, 255));
                al_draw_filled_rectangle(347, 417, 453, 448, al_map_rgb(255, 255, 255));
                al_draw_rectangle(285, 275, 515, 310, al_map_rgb(255, 255, 255), 3);
                al_draw_rectangle(210, 345, 590, 380, al_map_rgb(0,0,0), 3);
                al_draw_rectangle(345, 415, 455, 450, al_map_rgb(0,0,0), 3);

                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2-30 ,
                     ALLEGRO_ALIGN_CENTRE, "Restart");
                al_draw_text(font, al_map_rgb(0,0,0), WIDTH/2, HEIGHT/2+40 ,
                     ALLEGRO_ALIGN_CENTRE, "Back to menu");
                al_draw_text(font, al_map_rgb(0,0,0), WIDTH/2, HEIGHT/2+110 ,
                     ALLEGRO_ALIGN_CENTRE, "Exit");
                HP1--;
                HP2--;
            }
        }

        al_flip_display();
        //al_clear_to_color(al_map_rgb(0,0,0));

        // Listening for new event
        if (!al_is_event_queue_empty(event_queue)) {
            error = process_event();
        }
    }

    // Third window(About)
    else if(window == 3){
        al_draw_bitmap(background4, 0, 0, 0);
        // al_draw_bitmap(al_load_bitmap("background5.png"), 0, 0, 0);
        al_draw_bitmap(al_load_bitmap("background3.png"), 0, 0, 0);

        al_draw_text(al_load_ttf_font("pirulen.ttf",30,0), al_map_rgb(77,0,0),
                     400, 25 , ALLEGRO_ALIGN_CENTRE, "About the game");

        al_draw_bitmap(al_load_bitmap("Player1N.png"), 20, 90, 0);
        al_draw_bitmap(al_load_bitmap("Player2N.png"), 630, 90, 0);

        al_draw_bitmap(al_load_bitmap("Player1C.png"), 170, 140, 0);
        al_draw_bitmap(al_load_bitmap("Player2C.png"), 400, 140, 0);

        al_draw_text(al_load_ttf_font("pirulen.ttf",20,0), al_map_rgb(0,0,0),
                     260, 300 , ALLEGRO_ALIGN_CENTRE, "W:UP");
        al_draw_text(al_load_ttf_font("pirulen.ttf",20,0), al_map_rgb(0,0,0),
                     260, 350 , ALLEGRO_ALIGN_CENTRE, "S:DOWN");
        al_draw_text(al_load_ttf_font("pirulen.ttf",20,0), al_map_rgb(0,0,0),
                     260, 400 , ALLEGRO_ALIGN_CENTRE, "A:LEFT");
        al_draw_text(al_load_ttf_font("pirulen.ttf",20,0), al_map_rgb(0,0,0),
                     260, 450 , ALLEGRO_ALIGN_CENTRE, "D:RIGHT");
        al_draw_text(al_load_ttf_font("pirulen.ttf",20,0), al_map_rgb(0,0,0),
                     260, 520 , ALLEGRO_ALIGN_CENTRE, "J:ATTACK");

        al_draw_text(al_load_ttf_font("pirulen.ttf",20,0), al_map_rgb(0,0,0),
                     500, 300 , ALLEGRO_ALIGN_CENTRE, "UP:UP");
        al_draw_text(al_load_ttf_font("pirulen.ttf",20,0), al_map_rgb(0,0,0),
                     500, 350 , ALLEGRO_ALIGN_CENTRE, "DOWN:DOWN");
        al_draw_text(al_load_ttf_font("pirulen.ttf",20,0), al_map_rgb(0,0,0),
                     500, 400 , ALLEGRO_ALIGN_CENTRE, "LEFT:LEFT");
        al_draw_text(al_load_ttf_font("pirulen.ttf",20,0), al_map_rgb(0,0,0),
                     500, 450 , ALLEGRO_ALIGN_CENTRE, "RIGHT:RIGHT");
        al_draw_text(al_load_ttf_font("pirulen.ttf",20,0), al_map_rgb(0,0,0),
                     500, 520 , ALLEGRO_ALIGN_CENTRE, "1:ATTACK");

        al_draw_bitmap(Player1.image_path, Player1.x, Player1.y, 0);
        al_draw_bitmap(Player2.image_path, Player2.x, Player2.y, 1);

        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));

        // Listening for new event
        if (!al_is_event_queue_empty(event_queue)) {
            error = process_event();
        }
    }
    return error;
}

void Delay(int secs){
    int retTime;
    retTime = time(0) + secs;
    while (time(0) < retTime);
}

void reset(){
    G_begin = false;
    window = 1;
    judge_window2 = false;
    judge_window3 = false;
    dirP1=RIGHT;
    dirP2=LEFT;
    HP1=340;
    HP2=340;
    AtP1=0;
    AtP2=0;
    return;
}

void game_destroy(){
    // Make sure you destroy all things
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(background1);
    al_destroy_bitmap(background2);
    al_destroy_bitmap(background3);
    al_destroy_bitmap(background4);
    //al_destroy_timer(timer1);
    //al_destroy_timer(timer2);
    al_destroy_sample(song1);
    al_destroy_sample(song2);
    al_destroy_sample(song3);
    al_destroy_font(font);
}
