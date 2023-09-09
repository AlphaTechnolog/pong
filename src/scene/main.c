#include <SDL2/SDL_render.h>

#include "main.h"
#include "scene.h"

#include "../game/window.h"
#include "../entities/left_bar.h"
#include "../entities/right_bar.h"
#include "../utils.h"

#if !defined(MAIN_SCENE_ENTITIES_INDEXES)
#define MAIN_SCENE_ENTITIES_INDEXES

#define MAIN_SCENE_ENTITIES_LEFT_BAR 0
#define MAIN_SCENE_ENTITIES_RIGHT_BAR 1

#endif

void main_configure(void *window, struct Scene *scene) {
	CONFIGURE_SCENE(scene);
	scene->entities = malloc(sizeof(struct Bar));
	scene->entities[MAIN_SCENE_ENTITIES_LEFT_BAR] = create_left_bar((struct Window *) window);
	scene->entities[MAIN_SCENE_ENTITIES_RIGHT_BAR] = create_right_bar((struct Window*) window);
}

void main_draw(void *window, struct Scene *scene) {
	bar_render((struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_LEFT_BAR]);
	bar_render((struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_RIGHT_BAR]);
}

void main_destroy(void *window, struct Scene *scene) {
	free((struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_LEFT_BAR]);
	free((struct Bar*) scene->entities[MAIN_SCENE_ENTITIES_RIGHT_BAR]);
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