#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_render.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/extern/cJSON/cJSON.h"

#include "spritesheet.h"
#include "../utils.h"
#include "../game/window.h"

char *spritesheet_resolve_spritepath(const char *sprite_name) {
	unsigned int len = 0
		+ strlen("./sprites/")
		+ strlen(sprite_name)
		+ strlen(".json")
		+ 1;

	char *result = malloc(sizeof(char) * len);

	ASSERT(result, "Can't allocate memory!\n");
	sprintf(result, "%s/%s.json", "./sprites", sprite_name);

	return result;
}

char *spritesheet_resolve_texturepath(const char *sprite_name) {
	unsigned int len = 0
		+ strlen("./sprites/")
		+ strlen(sprite_name)
		+ strlen(".png")
		+ 1;

	char *result = malloc(sizeof(char) * len);

	ASSERT(result, "Can't allocate memory!\n");
	sprintf(result, "%s/%s.png", "./sprites", sprite_name);

	return result;
}

char *read_texture(const char *filename) {
	FILE *file = fopen(filename, "rb");
    long fsize = 0;

    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    rewind(file);

    char *filecontent = malloc(fsize + 1);

    fread(filecontent, fsize, 1, file);
    fclose(file);

    filecontent[fsize] = 0;

    return filecontent;
}

struct Frame **spritesheet_parse_texture_frames(struct Spritesheet *spritesheet, const char *texture_content) {
	cJSON *json = cJSON_Parse(texture_content);
	cJSON *frames_array = cJSON_GetObjectItem(json, "frames");

	spritesheet->max_frames = cJSON_GetArraySize(frames_array);

	struct Frame **frames = malloc(sizeof(struct Frame*) * spritesheet->max_frames);

	for (size_t i = 0; i < spritesheet->max_frames; ++i) {
		cJSON *frame = cJSON_GetObjectItem(cJSON_GetArrayItem(frames_array, i), "frame");

		struct Frame *new_frame = malloc(sizeof(struct Frame));

		new_frame->x = cJSON_GetNumberValue(cJSON_GetObjectItem(frame, "x"));
		new_frame->y = cJSON_GetNumberValue(cJSON_GetObjectItem(frame, "y"));
		new_frame->w = cJSON_GetNumberValue(cJSON_GetObjectItem(frame, "w"));
		new_frame->h = cJSON_GetNumberValue(cJSON_GetObjectItem(frame, "h"));

		frames[i] = new_frame;
	}

	cJSON_Delete(json);

	return frames;
}

struct Spritesheet *spritesheet_json_loader(struct Window *window, const char *texture_name) {
	char *spritepath = spritesheet_resolve_spritepath(texture_name);
	char *texturepath = spritesheet_resolve_texturepath(texture_name);

	CHECK_FOR_FILE(spritepath);
	CHECK_FOR_FILE(texturepath);

	char *texture_content = read_texture(spritepath);
	struct Spritesheet *spritesheet = malloc(sizeof(struct Spritesheet));

	spritesheet->frames = spritesheet_parse_texture_frames(spritesheet, texture_content);
	spritesheet->current_frame = 0;

	spritesheet->sprite_surface = IMG_Load(texturepath);

	ASSERT(spritesheet->sprite_surface, "Couldn't create the sprite surface!\n");

	spritesheet->sprite_texture = SDL_CreateTextureFromSurface(window->renderer,
		spritesheet->sprite_surface);

	ASSERT(spritesheet->sprite_texture, "Couldn't create the sprite texture!\n");

	free(spritepath);
	free(texturepath);
	free(texture_content);

	return spritesheet;
}

struct Frame **spritesheet_auto_parser(
	struct Spritesheet *spritesheet,
	SDL_Surface *surface,
	SDL_Point max_tiles,
	const int last_frames_skips
) {
	spritesheet->max_frames = (max_tiles.x * max_tiles.y)
		- (last_frames_skips < 0 ? 0 : last_frames_skips);

	SDL_Point tile_size = {
		.x = surface->w / max_tiles.x,
		.y = surface->h / max_tiles.y
	};

	struct Frame **frames = malloc(sizeof(struct Frame*) * spritesheet->max_frames);

	unsigned int i = 0;

	for (int y = 0; y < max_tiles.y; y++) {
		for (int x = 0; x < max_tiles.x; x++) {
			if (i >= spritesheet->max_frames) {
				break;
			}

			struct Frame *frame = malloc(sizeof(struct Frame));
			frame->x = x * tile_size.x;
			frame->y = y * tile_size.y;
			frame->w = tile_size.x;
			frame->h = tile_size.y;
			frames[i] = frame;
			i++;
		}
	}

	return frames;
}

struct Spritesheet *spritesheet_auto_loader(
	struct Window *window,
	SDL_Point max_tiles,
	const char *texture_name,
	const int last_frames_skips
) {
	char *texturepath = spritesheet_resolve_texturepath(texture_name);

	CHECK_FOR_FILE(texturepath);

	struct Spritesheet *spritesheet = malloc(sizeof(struct Spritesheet));

	spritesheet->sprite_surface = IMG_Load(texturepath);

	ASSERT(spritesheet->sprite_surface, "Couldn't create a spritesheet surface!\n");

	spritesheet->sprite_texture = SDL_CreateTextureFromSurface(window->renderer,
		spritesheet->sprite_surface);

	spritesheet->frames = spritesheet_auto_parser(spritesheet,
		spritesheet->sprite_surface,
		max_tiles,
		last_frames_skips);

	spritesheet->current_frame = 0;

	free(texturepath);

	return spritesheet;
}

SDL_Rect spritesheet_get_srcrect(struct Spritesheet *spritesheet) {
	struct Frame *current_frame = NULL;

	for (size_t i = 0; i < spritesheet->max_frames; ++i) {
		if (spritesheet->frames[i] != NULL && i == spritesheet->current_frame) {
			current_frame = (struct Frame*) spritesheet->frames[i];
		}
	}

	ASSERT(current_frame != NULL, "Cannot get current frame from cached frames!\n");

	return (SDL_Rect) {
		.x = current_frame->x,
		.y = current_frame->y,
		.w = current_frame->w,
		.h = current_frame->h
	};
}

void destroy_spritesheet(struct Spritesheet *spritesheet) {
	for (size_t i = 0; i < spritesheet->max_frames; ++i) {
		if (spritesheet->frames[i] != NULL) {
			free(spritesheet->frames[i]);
		}
	}

	SDL_FreeSurface(spritesheet->sprite_surface);
	SDL_DestroyTexture(spritesheet->sprite_texture);

	free(spritesheet);
}

