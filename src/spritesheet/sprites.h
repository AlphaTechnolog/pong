#include <SDL2/SDL_render.h>

#include "../game/window.h"

#pragma once

struct Sprite {
    SDL_Surface *surface;
    SDL_Texture *texture;
};

struct Sprite *create_sprite(struct Window *window, const char *sprite_name);
void destroy_sprite(struct Sprite *sprite);