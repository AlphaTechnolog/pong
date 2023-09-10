#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>

#include "main.h"
#include "scene.h"

#include "../game/window.h"
#include "../entities/left_bar.h"
#include "../entities/right_bar.h"
#include "../entities/ball.h"
#include "../utils.h"

#if !defined(MAIN_SCENE_ENTITIES_INDEXES)
#define MAIN_SCENE_ENTITIES_INDEXES

#define MAIN_SCENE_ENTITIES_LEFT_BAR 0
#define MAIN_SCENE_ENTITIES_RIGHT_BAR 1
#define MAIN_SCENE_ENTITIES_BALL 2

#endif

void main_configure(void *window, struct Scene *scene) {
	CONFIGURE_SCENE(scene);

	scene->entities = malloc(sizeof(struct Bar*) * 2 + sizeof(struct Ball*) * 1);
	scene->entities[MAIN_SCENE_ENTITIES_LEFT_BAR] = create_left_bar((struct Window*) window);
	scene->entities[MAIN_SCENE_ENTITIES_RIGHT_BAR] = create_right_bar((struct Window*) window);

	struct Bar **bars = malloc(sizeof(struct Bar*) * 2);

	bars[0] = (struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_LEFT_BAR];
	bars[1] = (struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_RIGHT_BAR];

	scene->entities[MAIN_SCENE_ENTITIES_BALL] = create_ball((struct Window*) window, bars);

	SDL_ShowCursor(SDL_DISABLE);
}

void main_draw(void *window, struct Scene *scene) {
	bar_render((struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_LEFT_BAR]);
	bar_render((struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_RIGHT_BAR]);
	ball_render((struct Ball*) scene->entities[MAIN_SCENE_ENTITIES_BALL]);
}

inline static void toggle_hitboxes(struct Bar* bars[2]) {
	for (uint16 i = 0; i < 2; ++i)
		bars[i]->show_debug_colliders = !bars[i]->show_debug_colliders;
}

// toggle hitboxes green rectangles when pressing f3
void main_handle_events(void *window, struct Scene *scene, SDL_Event *ev) {
	if (ev->type == SDL_KEYDOWN && ev->key.keysym.sym == SDLK_F3) {
		toggle_hitboxes((struct Bar*[]) {
			(struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_LEFT_BAR],
			(struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_RIGHT_BAR]
		});
	}
}

void main_destroy(void *window, struct Scene *scene) {
	free((struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_LEFT_BAR]);
	free((struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_RIGHT_BAR]);
	ball_destroy((struct Ball*) scene->entities[MAIN_SCENE_ENTITIES_BALL]);
	SDL_ShowCursor(SDL_ENABLE);
}

struct Scene *create_main_scene(void) {
	struct Scene *scene = malloc(sizeof(struct Scene));

	scene->configured = 0;
	scene->configure = main_configure;
	scene->draw = main_draw;
	scene->handle_events = main_handle_events;
	scene->destroy = main_destroy;

	return scene;
}