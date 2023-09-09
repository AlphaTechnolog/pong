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
}

void main_draw(void *window, struct Scene *scene) {
	bar_render((struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_LEFT_BAR]);
	bar_render((struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_RIGHT_BAR]);
	ball_render((struct Ball*) scene->entities[MAIN_SCENE_ENTITIES_BALL]);
}

void main_destroy(void *window, struct Scene *scene) {
	free((struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_LEFT_BAR]);
	free((struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_RIGHT_BAR]);
	ball_destroy((struct Ball*) scene->entities[MAIN_SCENE_ENTITIES_BALL]);
}

struct Scene *create_main_scene(void) {
	struct Scene *scene = malloc(sizeof(struct Scene));

	scene->configured = 0;
	scene->configure = main_configure;
	scene->draw = main_draw;
	scene->handle_events = scene_undefined_handle_event_callback;
	scene->destroy = main_destroy;

	return scene;
}