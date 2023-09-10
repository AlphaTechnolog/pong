#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>

#include "../game/window.h"
#include "../utils.h"

#pragma once

struct TextParams {
	struct Window *window;
	const char *fontpath;
	const char *text;
	const unsigned int ptsize;
	SDL_Color color;
	SDL_Rect *dst;
};

SDL_Texture *fetch_text_texture(struct TextParams prms);
void render_text_texture(struct Window *window, SDL_Texture *texture, SDL_Rect dst);
void render_text(struct TextParams prms);