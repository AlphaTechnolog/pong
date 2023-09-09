#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "loop.h"

#include "../config.h"
#include "../scene/main.h"

unsigned int quit = 0;
unsigned int last_frame_time = 0;

void handle_keys(struct Window *window) {
    SDL_Event ev;

    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
            quit = 1;
        }

        if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_F11) {
            window_toggle_fullscreen(window);
        }

        if (ev.type == SDL_WINDOWEVENT) {
            window_fetch_position(window);
        }

        if (ev.type == SDL_MOUSEMOTION) {
            window_fetch_mouse_coords(window, &ev);
        }

        if (ev.type == SDL_WINDOWEVENT &&
            ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            window_refetch_dimensions(window);
        }

        window->current_scene->handle_events(window, window->current_scene,
                                             &ev);
    }
}

void fps_capping(void) {
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    last_frame_time = SDL_GetTicks();
}

void pre_update(struct Window *window) {
    SDL_RenderClear(window->renderer);
    window->ticks = SDL_GetTicks();
}

void render_scene(struct Window *window) {
    window->current_scene->configure(window, window->current_scene);
    window->current_scene->draw(window, window->current_scene);
    if (window->render_scene == 0) // rendering scene-changing transition.
        window_scene_transition_splash(window);
}

void post_update(struct Window *window) {
    SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 0);
    SDL_RenderPresent(window->renderer);
}

void game_loop(struct Window *window) {
    window->current_scene = create_main_scene();
    window->tmp_scene = window->current_scene;

    while (quit == 0) {
        handle_keys(window);
        fps_capping();
        pre_update(window);
        render_scene(window);
        post_update(window);
    }

    window->current_scene->destroy(window, window->current_scene);
}