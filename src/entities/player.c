#include "player.h"
#include "../game/window.h"
#include "../lib/vector/vector.h"

void player_update_collider(struct Player *player) {
	player->collider = (SDL_Rect) {
		.x = player->pos.x,
		.y = player->pos.y,
		.w = player->size.x,
		.h = player->size.y
	};
}

struct Player *create_player(struct Window *window) {
	struct Player *player = malloc(sizeof(struct Player));

	player->window = window;
	player->size = (Vector2d) {60.0f, 60.0f};

	player->pos = (Vector2d) {
		.x = (player->window->width - player->size.x) / 2,
		.y = (player->window->height - player->size.y) / 2
	};

	player_update_collider(player);

	return player;
}

void player_handle_movement(struct Player *player) {
	Vector2d mouse_coords = window_get_mouse_coords_vec2d(player->window);
	mouse_coords.x -= player->size.x / 2;
	mouse_coords.y -= player->size.y / 2;
	copy_vec2d(&mouse_coords, &player->pos);
}

void player_render(struct Player *player) {
	player_handle_movement(player);
	player_update_collider(player);
	SDL_SetRenderDrawColor(player->window->renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderDrawRect(player->window->renderer, &player->collider);
	SDL_RenderFillRect(player->window->renderer, &player->collider);
}