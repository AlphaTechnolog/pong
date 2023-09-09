#include <SDL2/SDL_rect.h>

#include "../game/window.h"
#include "../lib/vector/vector.h"

#pragma once

struct Player {
	Vector2d pos;
	Vector2d size;
	SDL_Rect collider;
	struct Window *window; // actual window instance
};

struct Player *create_player(struct Window *window);
void player_render(struct Player *player);
