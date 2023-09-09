#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_keyboard.h>

#include <string.h>

#include "bar.h"

#include "../lib/vector/vector.h"
#include "../utils.h"
#include "../config.h"

void bar_update_collider(struct Bar *bar) {
	bar->collider = (SDL_Rect) {
		.x = bar->pos.x,
		.y = bar->pos.y,
		.w = bar->size.x,
		.h = bar->size.y
	};
}

struct Bar *create_bar(struct Window *window) {
	struct Bar *bar = malloc(sizeof(struct Bar));

	bar->window = window;
	bar->pos = (Vector2d) {0.0f, 0.0f};

	bar->size = (Vector2d) {
		.x = 15,
		.y = 80
	};

	bar_update_collider(bar);

	return bar;
}

#define BAR_CONTROL_T(bar) __typeof__((bar)->controls[0])*
void bar_handle_movement(struct Bar *bar) {
	BAR_CONTROL_T(bar) bar_control_up;
	BAR_CONTROL_T(bar) bar_control_down;

	for (size_t i = 0; i < sizeof(bar->controls) / sizeof(bar->controls[0]); ++i) {
		if (strcmp(bar->controls[i].where, BAR_WHERE_TOP) == 0) {
			bar_control_up = &bar->controls[i];
		}
		if (strcmp(bar->controls[i].where, BAR_WHERE_DOWN) == 0) {
			bar_control_down = &bar->controls[i];
		}
	}

	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	const uint16 ACC = 6;

	if (keystate[bar_control_up->scancode]) bar->pos.y -= ACC;
	if (keystate[bar_control_down->scancode]) bar->pos.y += ACC;

	if (bar->pos.y <= WINGAP) bar->pos.y = WINGAP;

	if (bar->pos.y + bar->size.y >= bar->window->height - WINGAP)
		bar->pos.y = bar->window->height - WINGAP - bar->size.y;
}

void bar_render(struct Bar *bar) {
	bar_handle_movement(bar);
	bar_update_collider(bar);
	SDL_SetRenderDrawColor(bar->window->renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderDrawRect(bar->window->renderer, &bar->collider);
	SDL_RenderFillRect(bar->window->renderer, &bar->collider);
}