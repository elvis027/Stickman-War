#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

ALLEGRO_DISPLAY* display = NULL;

ALLEGRO_FONT* font = NULL;
ALLEGRO_BITMAP* img = NULL;

const int width = 800;
const int height = 600;

const int img_width = 640;
const int img_height = 480;

void show_err_msg(int msg);
void game_init();
void game_begin();
void game_destroy();

int main(int argc, char *argv[])
{
    int msg = 0;

    game_init();
    game_begin();
    printf("Hello world!!!\n");
    al_rest(10);

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
        show_err_msg(-1);
    }

    display = al_create_display(width, height);
    if(display == NULL) {
        show_err_msg(-1);
    }

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon();

    font = al_load_font("pirulen.ttf", 12, 0);
    img = al_load_bitmap("htchen.jpg");
    if(font == NULL || img == NULL) {
        show_err_msg(-1);
    }
}

void game_begin()
{
    al_clear_to_color(al_map_rgb(100, 100, 100));

    al_draw_text(font, al_map_rgb(255, 255, 255), img_width - 80, img_height + 20,
                    ALLEGRO_ALIGN_CENTER, "Why?");
    al_draw_text(font, al_map_rgb(255, 255, 255), img_width - 80, img_height + 40,
                    ALLEGRO_ALIGN_CENTER, "Why am I the smartest professor in the NTHU?");

    al_draw_rectangle(img_width - 320, img_height + 10, width - 10, img_height + 70,
                    al_map_rgb(255, 255, 255), 0);

    al_draw_bitmap(img, 0, 0, 0);

    al_flip_display();
}

void game_destroy()
{
    al_destroy_display(display);
    al_destroy_font(font);
    al_destroy_bitmap(img);
}
