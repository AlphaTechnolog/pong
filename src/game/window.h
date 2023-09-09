#include <SDL2/SDL.h>
#include <string.h>

#include "../scene/scene.h"
#include "../lib/vector/vector.h"

#pragma once

struct Window {
    // sdl window instance
    SDL_Window *window;

    // sdl window renderer
    SDL_Renderer *renderer;

    // is the window in fullscreen mode?
    unsigned int is_fullscreen;

    // actual window width
    int width;

    // actual window height
    int height;

    // mouse x
    int mouse_x;

    // mouse y
    int mouse_y;

    // global mouse x (position relative to the desktop itself.)
    int global_mouse_x;

    // global mouse y (position relative to the desktop itself.)
    int global_mouse_y;

    // actual window x
    int x;

    // actual window y
    int y;

    // the current app ticks
    int ticks;

    // hooks: hooks is a concept that allows some parts of the game
    // manipulate or execute code when some event in the window
    // occurs, such as window dimensions changed, or mouse position changed.
    struct {
        char *id; // id such as window.dimensions::changed (e.g)
        void *meta; // some struct or info you would like to preserve when calling the hook.
        void (*handler)(struct Window *window, void *meta, void *prm);
    } **hooks;

    // number of registered hooks
    unsigned int hooks_length;

    /* == scene manipulation flow variables == */

    // the current rendered scene
    struct Scene *current_scene;

    // a temporal backup scene for the transition.
    struct Scene *tmp_scene;

    // tells the game loop if it should render or doesn't
    // should render the actual scene.
    unsigned int render_scene;

    // scene transition desired ticks to complete
    int desired_scene_transition_ticks;

    // ticks when the scene transition started.
    int scene_transition_start_ticks;

    // tells if the animation should go backwards or not.
    unsigned int scene_transition_go_backwards;
};

#define HOOK_T(window) __typeof__(*((window)->hooks[0]))

// FIXME: For some reason, this macro is giving errors, so
// for now, let's switch into a regular static function. see: window.c
// and search for the function `resolve_hook`.

// #define RESOLVE_HOOK(window, signal, prm)                         \
//     for (size_t i = 0; i < (window)->hooks_length; ++i) {         \
//         HOOK_T(window) *hook = (window)->hooks[i];                \
//         if (hook && strcmp(hook->id, signal) == 0) {              \
//             hook->handler((window), hook->meta, (prm));           \
//         }                                                         \
//     }

struct Window *create_window(void);
void window_toggle_fullscreen(struct Window *window);
void window_scene_transition_splash(struct Window *window);
void window_switch_scene(struct Window *window, struct Scene *current_scene);
void window_fetch_position(struct Window *window);
void window_refetch_dimensions(struct Window *window);
void window_fetch_mouse_coords(struct Window *window, SDL_Event *ev);
Vector2d window_get_mouse_coords_vec2d(struct Window *window);
void window_register_hook(struct Window *window, HOOK_T(window) *hook);
void window_unregister_hook(struct Window *window, HOOK_T(window) *hook);
void destroy_window(struct Window *window);
