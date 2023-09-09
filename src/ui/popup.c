#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

#include <stdio.h>
#include <stdlib.h>

#include "popup.h"

#include "../lib/vector/vector.h"

void update_popup_rect(struct Popup *popup) {
    popup->rect = (SDL_Rect) {
        .x = popup->coordinates->x,
        .y = popup->coordinates->y,
        .w = popup->size->x,
        .h = popup->size->y
    };
}

void render_popup(struct Popup *popup) {
    update_popup_rect(popup);

    SDL_SetRenderDrawBlendMode(popup->window->renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(popup->window->renderer,
        popup->color.r,
        popup->color.g,
        popup->color.b,
        popup->color.a);

    SDL_RenderDrawRect(popup->window->renderer, &popup->rect);
    SDL_RenderFillRect(popup->window->renderer, &popup->rect);
}

struct Popup *create_popup(
    struct Window *window,
    Vector2d *coordinates,
    Vector2d *size,
    SDL_Color color
) {
    struct Popup *popup = malloc(sizeof(struct Popup));

    popup->window = window;
    popup->coordinates = coordinates;
    popup->size = size;
    popup->color = color;

    update_popup_rect(popup);

    return popup;
}

void destroy_popup(struct Popup *popup) {
    free(popup->coordinates);
    free(popup->size);
    free(popup);
}
