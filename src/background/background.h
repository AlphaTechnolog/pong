#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

#include "../game/window.h"

#pragma once

#define BACKGROUND_MODE_STRETCH  1  // stretchs an image to fill the whole window.
#define BACKGROUND_MODE_TILE 	 2  // repeats a little image in the whole background renderer.
#define BACKGROUND_MODE_COLOR    3  // draws a solid color in the background using a rectangle.

struct Background {
	// the application window instance.
	struct Window *window;

	// defines if the mode of rendering for the background
	// currently allows `BACKGROUND_MODE_*`
	unsigned int mode;

	// solid color value.
	SDL_Color color;

	// the sdl surface of the image itself.
	SDL_Surface *surface;

	// the sdl texture of the `surface` property.
	SDL_Texture *texture;
};

struct Background *create_background(struct Window *window, void *value, const unsigned int mode);

void render_background(struct Background *background);

void destroy_background(struct Background *background);

