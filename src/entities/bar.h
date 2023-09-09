#include <SDL2/SDL_rect.h>

#include <string.h>

#include "../game/window.h"
#include "../lib/vector/vector.h"
#include "../utils.h"

#pragma once

#define BAR_WHERE_TOP "top"
#define BAR_WHERE_DOWN "down"

#define DEFCONTROLS(bar, up, down)									\
	(bar)->controls[0] = (__typeof__((bar)->controls[0])) { 		\
		.where = BAR_WHERE_TOP,										\
		.scancode = (up)											\
	};																\
	(bar)->controls[1] = (__typeof__((bar)->controls[1])) {			\
		.where = BAR_WHERE_DOWN,									\
		.scancode = (down)											\
	};

#define BAR_ID_LEFT "left"
#define BAR_ID_RIGHT "right"

#define IS_BAR_ID(bar, pssblid) strcmp((bar), (pssblid)) == 0

struct Bar {
	Vector2d pos;
	Vector2d size;
	SDL_Rect collider;
	uint128 score;

	struct Window *window;

	char *id;

	struct {
		char *where;
		int scancode;
	} controls[2];
};

struct Bar *create_bar(struct Window *window, char *id);
void bar_render(struct Bar *bar);