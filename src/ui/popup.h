// this module provides a way to create/manage popups made through
// sdl rects in a reliable way.

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>

#include "../game/window.h"
#include "../lib/vector/vector.h"

#pragma once

struct Popup {
    // the app object.
    struct Window *window;

    // the coordinates of the popup itself.
    Vector2d *coordinates;

    // the size of the popup.
    Vector2d *size;

    // the color of the popup rect.
    SDL_Color color;

    // the rect of the popup.
    SDL_Rect rect;
};

struct Popup *create_popup(
    struct Window *window,
    Vector2d *coordinates,
    Vector2d *size,
    SDL_Color color
);

void update_popup_rect(struct Popup *popup);

void render_popup(struct Popup *popup);

void destroy_popup(struct Popup *popup);
