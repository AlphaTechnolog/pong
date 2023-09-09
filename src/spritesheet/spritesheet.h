// spritesheets texture.json files are being generated using
// free-tex-packer.com/app, so we need to parse and store the calculated
// frames somewhere, allowing us to easily load/use spritesheets.

// this spritesheet library also supports automatic spritesheet (full image)
// parsing! but you'll need to specify things like, the max frames and frames to skip, etc
// however, this still remains as the preferred method as it's more flexible.

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_render.h>

#include "../game/window.h"

#pragma once

#define SPRITESHEET_AUTO_NO_SKIP -1

// represents the exported frame item
struct Frame {
	int x;
	int y;
	int w;
	int h;
};

// spritesheet object.
struct Spritesheet {
	// represents the parsed frames with their
	// coordinates to cut on.
	struct Frame **frames;

	// available max frames
	unsigned int max_frames;

	// in-memory full texture spritesheet-image (surface & renderer).
	SDL_Surface *sprite_surface;
	SDL_Texture *sprite_texture;

	// current frame index
	unsigned int current_frame;
};

// json loader.
struct Frame **spritesheet_parse_texture_frames(
	struct Spritesheet *spritesheet,
	const char *texture_content
);

struct Spritesheet *spritesheet_json_loader(struct Window *window, const char *texture_name);

// auto parser.
struct Frame **spritesheet_auto_parser(
	struct Spritesheet *spritesheet,
	SDL_Surface *surface,
	SDL_Point max_tiles,
	const int last_frames_skips
);

struct Spritesheet *spritesheet_auto_loader(
	struct Window *window,
	SDL_Point max_tiles,
	const char *texture_name,
	const int last_frames_skips
);

// general api.
SDL_Rect spritesheet_get_srcrect(struct Spritesheet *spritesheet);

void destroy_spritesheet(struct Spritesheet *spritesheet);

