#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "loop.h"

#include "../config.h"
#include "../game/window.h"
#include "../scene/menu.h"
#include "../ui/metrics.h"

unsigned int quit = 0;
unsigned int last_frame_time = 0;
unsigned int frame_count = 0;
unsigned int last_fps_update = 0;

#if defined(DEBUG_ENABLE_METRICS)
void handle_keys(struct Window *window, struct Metrics *metrics) {
#else
void handle_keys(struct Window *window) {
#endif
    SDL_Event ev;

    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) quit = 1;

        if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_F11)
            window_toggle_fullscreen(window);
        if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_F3)
            window->stats_for_nerds_visible = !window->stats_for_nerds_visible;

        if (ev.type == SDL_WINDOWEVENT)
            window_fetch_position(window);

        if (ev.type == SDL_MOUSEMOTION)
            window_fetch_mouse_coords(window, &ev);

        if (ev.type == SDL_WINDOWEVENT &&
            ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            window_refetch_dimensions(window);
        }

        window->current_scene->handle_events(window,
            window->current_scene,
            &ev);

        #if defined(DEBUG_ENABLE_METRICS)
            metrics_handle_events(metrics, &ev);
        #endif
    }
}

static void update_fps(struct Window *window) {
    int current_time = SDL_GetTicks();
    int elapsed_time = current_time - last_fps_update;

    if (elapsed_time >= 1000) {
        window->fps = frame_count / (elapsed_time / 1000.0);
        frame_count = 0;
        last_fps_update = current_time;
    }
}

void fps_capping(struct Window *window) {
    frame_count++;
    update_fps(window);

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
    window->current_scene = create_menu_scene();
    window->tmp_scene = window->current_scene;

    #if defined(FULLSCREEN_BY_DEFAULT)
        window_toggle_fullscreen(window);
    #endif

    #if defined(DEBUG_ENABLE_METRICS)
        struct Metrics *metrics = create_metrics(window);
    #endif

    while (quit == 0) {
        #if defined(DEBUG_ENABLE_METRICS)
            handle_keys(window, metrics);
        #else
            handle_keys(window);
        #endif
        fps_capping(window);
        pre_update(window);
        #if defined(DEBUG_ENABLE_METRICS)
            render_metrics(metrics);
            render_last_msg(window);
        #endif
        render_scene(window);
        post_update(window);
    }

    window->current_scene->destroy(window, window->current_scene);

    #if defined(DEBUG_ENABLE_METRICS)
        destroy_metrics(metrics);
    #endif
}