#include <SDL2/SDL_events.h>

#pragma once

struct Scene {
	// has this scene been already configured?
	unsigned int configured;

	// configures the scene itself (should be executed one time only, use `configured`).
	void (*configure)(void *window, struct Scene *scene);

	// draw entities in the scene.
	void (*draw)(void *window, struct Scene *scene);

	// handle sdl events callback.
	void (*handle_events)(void *window, struct Scene *scene, SDL_Event *ev);

	// destroys the scene entities.
	void (*destroy)(void *window, struct Scene *scene);

	// music soundtracks.
	void **audio_effects;

	// used entities in the current scene.
	void **entities;

	// ui elements loaded in memory.
	void **ui;
};

void scene_undefined_callback(void *window, struct Scene *scene);

void scene_undefined_handle_event_callback(void *window, struct Scene *scene, SDL_Event *ev);

