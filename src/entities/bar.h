#include <SDL2/SDL_rect.h>

#include "../game/window.h"
#include "../lib/vector/vector.h"

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

struct Bar {
	Vector2d pos;
	Vector2d size;
	SDL_Rect collider;
	struct Window *window;

	struct {
		char *where;
		int scancode;
	} controls[2];
};

struct Bar *create_bar(struct Window *window);
void bar_render(struct Bar *bar);