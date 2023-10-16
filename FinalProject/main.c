#include "function.h"

int main(int argc, char* argv[])
{
    GameInit();
    GameBegin();

    Message msg = GAME_CONTINUE;

    while(msg != GAME_TERMINATE) {
        msg = GameRun();
    }

    printf("Game Over\n");
    GameDestroy();

    return 0;
}
