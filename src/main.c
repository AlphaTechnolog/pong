#include <stdio.h>

#include "game/window.h"
#include "game/loop.h"

int main(int argc, char **argv) {
    struct Window *window = create_window();
    game_loop(window);
    destroy_window(window);
    return 0;
}

