#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "background.h"
#include "../game/window.h"
#include "../utils.h"

char *background_resolve_texturepath(const char *filename) {
	int length = 1
		+ strlen("sprites/")
		+ strlen(filename)
		+ strlen(".png");

	char *result = malloc(sizeof(char) * length);
	ASSERT(result, "Couldn't allocate memory!\n");
	sprintf(result, "%s%s.png", "sprites/", filename);
	return result;
}

void background_load_image(struct Background *background, const char *filename) {
	char *texture_path = background_resolve_texturepath(filename);

	CHECK_FOR_FILE(texture_path);

	background->surface = IMG_Load(texture_path);

	background->texture = SDL_CreateTextureFromSurface(background->window->renderer,
		background->surface);

	free(texture_path);

	ASSERT(background->surface, "Couldn't create the background SDL surface: %s\n",
		IMG_GetError());

	ASSERT(background->texture, "Couldn't convert the surface into a texture: %s\n",
		SDL_GetError());
}

struct Background *create_background(struct Window *window, void *value, const unsigned int mode) {
	struct Background *background = malloc(sizeof(struct Background));

	background->mode = mode;
	background->window = window;

	if (background->mode != BACKGROUND_MODE_COLOR) {
		background_load_image(background, (const char *)value);
	} else {
		SDL_Color *c = (SDL_Color*) value;
		background->color = *c;
	}

	return background;
}

void background_render_stretched(struct Background *background) {
	SDL_Rect rect = (SDL_Rect) {
		.x = 0,
		.y = 0,
		.w = background->window->width,
		.h = background->window->height
	};

	SDL_RenderCopy(background->window->renderer, background->texture,
		NULL, &rect);
}

void background_render_tiled(struct Background *background) {
	size_t tile_x_amount = background->window->width / background->surface->w;
	size_t tile_y_amount = background->window->height / background->surface->h;

	// checking decimal error
	while (tile_x_amount * background->surface->w < background->window->width) {
		tile_x_amount++;
	}

	while (tile_y_amount * background->surface->h < background->window->height) {
		tile_y_amount++;
	}

	// actually rendering
	for (size_t y = 0; y < tile_y_amount; ++y) {
		for (size_t x = 0; x < tile_x_amount; ++x) {
			SDL_Rect dst = {
				.x = x * background->surface->w,
				.y = y * background->surface->h,
				.w = background->surface->w,
				.h = background->surface->h
			};

			SDL_RenderCopy(background->window->renderer, background->texture,
				NULL, &dst);
		}
	}
}

void background_render_color(struct Background *background) {
	SDL_Rect dst = {0, 0, background->window->width, background->window->height};

	SDL_SetRenderDrawColor(background->window->renderer, background->color.r,
		background->color.g, background->color.b,
		background->color.a);

	SDL_RenderDrawRect(background->window->renderer, &dst);
	SDL_RenderFillRect(background->window->renderer, &dst);
}

void render_background(struct Background *background) {
	if (background->mode == BACKGROUND_MODE_STRETCH) {
		background_render_stretched(background);
	} else if (background->mode == BACKGROUND_MODE_TILE) {
		background_render_tiled(background);
	} else if (background->mode == BACKGROUND_MODE_COLOR) {
		background_render_color(background);
	} else {
		fprintf(stderr, "Invalid given background mode, aborting rendering!\n");
		destroy_background(background);
	}
}

void destroy_background(struct Background *background) {
	if (background->mode != BACKGROUND_MODE_COLOR) {
		SDL_FreeSurface(background->surface);
		SDL_DestroyTexture(background->texture);
	}

	free(background);
}

