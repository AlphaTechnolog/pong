#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sprites.h"

#include "../game/window.h"
#include "../utils.h"

static char *resolve_spritename(const char *basefile) {
    char *result = malloc(sizeof(char) * strlen("./sprites/") + strlen(basefile) + strlen(".png") + 1);
    sprintf(result, "./sprites/%s.png", basefile);
    return result;
}

struct Sprite *create_sprite(struct Window *window, const char *sprite_name) {
    char *filename = resolve_spritename(sprite_name);
    ASSERT(filename, "Couldn't load sprite with name == %s\n", sprite_name);

    struct Sprite *sprite = malloc(sizeof(struct Sprite));

    sprite->surface = IMG_Load(filename);    

    sprite->texture = SDL_CreateTextureFromSurface(window->renderer,
        sprite->surface);

    ASSERT(sprite->surface, "Can't load surface for sprite with file == %s\n",
        filename);

    ASSERT(sprite->texture, "Can't load texture for sprite with file == %s\n",
        filename);

    free(filename);
    
    return sprite;
}

void destroy_sprite(struct Sprite *sprite) {
    SDL_FreeSurface(sprite->surface);
    SDL_DestroyTexture(sprite->texture);
    free(sprite);
}