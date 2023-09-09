#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_rect.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "text.h"
#include "../game/window.h"
#include "../utils.h"

char *resolve_filename(const char *path) {
	const unsigned int length = 1
		+ strlen("fonts/")
		+ strlen(path)
		+ strlen(".ttf");

	char *result = malloc(sizeof(char) * length);
	ASSERT(result, "Couldn't allocate memory!\n");
	sprintf(result, "fonts/%s.ttf", path);
	return result;
}

SDL_Texture *fetch_text_texture(struct TextParams prms) {
	char *fontpath = resolve_filename(prms.fontpath);

	CHECK_FOR_FILE(fontpath);

	TTF_Font *opened_font = TTF_OpenFont(fontpath, prms.ptsize);

	free(fontpath);

	SDL_Surface *surface = TTF_RenderText_Blended(opened_font, prms.text,
		prms.color);

	TTF_CloseFont(opened_font);

	SDL_Texture *texture = SDL_CreateTextureFromSurface(prms.window->renderer,
		surface);

	prms.dst->w = surface->w;
	prms.dst->h = surface->h;

	SDL_FreeSurface(surface);

	return texture;
}

void render_text_texture(struct Window *window, SDL_Texture *texture, SDL_Rect dst) {
	SDL_RenderCopy(window->renderer, texture, NULL,
		&dst);
}

void render_text(struct TextParams prms, SDL_Rect dst) {
	SDL_Texture *texture = fetch_text_texture(prms);
	render_text_texture(prms.window, texture, dst);
	SDL_DestroyTexture(texture);
}

