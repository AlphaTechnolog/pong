#include "left_bar.h"
#include "bar.h"

#include "../lib/vector/vector.h"
#include "../game/window.h"
#include "../config.h"

struct Bar *create_left_bar(struct Window *window) {
	struct Bar *left_bar = create_bar(window, BAR_ID_LEFT);

	left_bar->pos = (Vector2d) {
		.x = WINGAP,
		.y = (left_bar->window->height - left_bar->size.y) / 2
	};

	DEFCONTROLS(left_bar, SDL_SCANCODE_W, SDL_SCANCODE_S);

	return left_bar;
}