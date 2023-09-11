#define SDL_MAIN_HANDLED

#include <stdio.h>

#include "game/window.h"
#include "game/loop.h"

int main(void) {
    struct Window *window = create_window();
    game_loop(window);
    destroy_window(window);
    return 0;
}

