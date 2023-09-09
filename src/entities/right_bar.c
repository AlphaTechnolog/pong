#include "right_bar.h"
#include "bar.h"

#include "../lib/vector/vector.h"
#include "../game/window.h"
#include "../config.h"

// re positionates the right bar `pos` vector when resizing the window object.
void right_bar_on_window_dimensions_change(struct Window *window, void *meta, void *prm) {
    struct Bar *right_bar = (struct Bar *) meta;
    right_bar->pos.x = right_bar->window->width - WINGAP - right_bar->size.x;
}

struct Bar *create_right_bar(struct Window *window) {
    struct Bar *right_bar = create_bar(window, BAR_ID_RIGHT);

    right_bar->pos = (Vector2d) {
        .x = right_bar->window->width - WINGAP - right_bar->size.x,
        .y = (right_bar->window->height - right_bar->size.y) / 2
    };

    DEFCONTROLS(right_bar, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);

    HOOK_T(right_bar->window) *hook = malloc(sizeof(HOOK_T(right_bar->window)));

    hook->id = "window.dimensions::changed";
    hook->meta = (void*) right_bar;
    hook->handler = right_bar_on_window_dimensions_change;

    window_register_hook(right_bar->window, hook);

    return right_bar;
}

