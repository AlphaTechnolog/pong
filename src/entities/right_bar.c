#include "right_bar.h"
#include "bar.h"

#include "../lib/vector/vector.h"
#include "../game/window.h"
#include "../config.h"

// re positionates the right bar `pos` vector when resizing the window object.
static void on_dimensions_change(struct Window *window, void *meta, void *prm) {
    struct Bar *right_bar = (struct Bar *) meta;
    Vector2d *size = (Vector2d *) prm;
    right_bar->pos.x = size->x - WINGAP - right_bar->size.x;
}

struct Bar *create_right_bar(struct Window *window) {
    struct Bar *right_bar = create_bar(window, BAR_ID_RIGHT);

    right_bar->pos = (Vector2d) {
        .x = right_bar->window->width - WINGAP - right_bar->size.x,
        .y = (right_bar->window->height - right_bar->size.y) / 2
    };

    DEFCONTROLS(right_bar, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);

    // subscribing to the window dimensions in order to make the position responsive
    // per device (well, mostly when going no fullscreen).
    def_winhook(right_bar->window, &(HOOK_T(right_bar->window)) {
        .id = "window.dimensions::changed",
        .meta = right_bar,
        .handler = on_dimensions_change
    });

    return right_bar;
}

