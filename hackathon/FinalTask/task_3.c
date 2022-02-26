#include <stdio.h>
#include <allegro5/allegro.h>

#define GAME_TERMINATE 666

#define AL_INIT_FAILED -1
#define DESPLAY_INIT_FAILED -2

ALLEGRO_DISPLAY* display = NULL;

ALLEGRO_EVENT_QUEUE* event_queue = NULL;
ALLEGRO_EVENT event;

const int width = 800;
const int height = 600;

void show_err_msg(int msg);
void game_init();
void game_begin();
int process_event();
int game_run();
void game_destroy();

int main(int argc, char *argv[])
{
    int msg = 0;

    game_init();
    game_begin();
    printf("Hello world!!!\n");
    printf("Close window to terminate.\n");

    while(msg != GAME_TERMINATE) {
        msg = game_run();
        if(msg == GAME_TERMINATE)
            printf("See you, world\n");
    }

    game_destroy();
    return 0;
}

void show_err_msg(int msg)
{
    fprintf(stderr, "unexpected msg: %d", msg);
    game_destroy();
    exit(9);
}

void game_init()
{
    if(!al_init()) {
        show_err_msg(AL_INIT_FAILED);
    }

    display = al_create_display(width, height);
    if(display == NULL) {
        show_err_msg(DESPLAY_INIT_FAILED);
    }

    event_queue = al_create_event_queue();
    if(event_queue == NULL) {
        show_err_msg(-1);
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
}

void game_begin()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
}

int process_event()
{
    al_wait_for_event(event_queue, &event);

    if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return GAME_TERMINATE;
    else
        return -1;
}

int game_run()
{
    int msg = 0;
    if(!al_is_event_queue_empty(event_queue)) {
        msg = process_event();
    }
    return msg;
}

void game_destroy()
{
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}
