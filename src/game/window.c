#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "window.h"

#include "../ui/metrics.h"
#include "../config.h"
#include "../utils.h"
#include "../texts/text.h"
#include "../lib/vector/vector.h"

void console_log(struct Window *window, const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), msg, args);
    va_end(args);
    window->last_console_msg = realloc(window->last_console_msg, sizeof(char) * strlen(buffer) + 1);
    sprintf(window->last_console_msg, "%s", buffer);
    printf("%s\n", window->last_console_msg);
}

#define PREFIX "> "
void render_last_msg(struct Window *window) {
    if (window->stats_for_nerds_visible == 0) return;

    const uint16 size = 16;
    char *buf = malloc(sizeof(char) * strlen(PREFIX) + strlen(window->last_console_msg) + 1);
    sprintf(buf, "%s%s", PREFIX, window->last_console_msg);

    render_text((struct TextParams) {
        .window = window,
        .text = buf,
        .ptsize = size,
        .dst = &(SDL_Rect) {WINGAP, window->height - size - WINGAP},
        .color = (SDL_Color) {0xff, 0xff, 0xff, 0xff},
        .fontpath = "retro"
    });

    free(buf);
}
#undef PREFIX

void window_fetch_position(struct Window *window) {
    SDL_GetWindowPosition(window->window, &window->x, &window->y);
}

struct Window *create_window(void) {
    struct Window *window = malloc(sizeof(struct Window));

    printf("Welcome to Alpha's Engine... Initializing\n");

    ASSERT(SDL_Init(SDL_INIT_EVERYTHING) >= 0,
           "Couldn't initialize sdl due to %s\n", SDL_GetError());

    ASSERT(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) >= 0,
           "Couldn't initialize images backend: %s\n", IMG_GetError());

    ASSERT(TTF_Init() >= 0, "Couldn't initialize text backend: %s\n",
           TTF_GetError());

    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    printf("[SDL] Compiled SDL version %u.%u.%u\n",
        compiled.major, compiled.minor, compiled.patch);

    printf("[SDL] Linked SDL version %u.%u.%u\n",
        linked.major, linked.minor, linked.patch);

    int mix_result = 0;
    int mix_flags = MIX_INIT_MP3;

    ASSERT(mix_flags == (mix_result = Mix_Init(mix_flags)),
           "Couldn't initialize mixer (result: %d): %s!\n", mix_result,
           Mix_GetError());

    window->window = SDL_CreateWindow(
        WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    window->renderer =
        SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);

    window->is_fullscreen = 0;
    window->render_scene = 1;

    // scene transition flow variables.
    window->desired_scene_transition_ticks = -1;
    window->scene_transition_start_ticks = -1;
    window->scene_transition_go_backwards = 0;

    ASSERT(window->window, "Couldn't create a SDL window!\n");
    ASSERT(window->renderer, "Couldn't create a SDL renderer!\n");

    window->width = WINDOW_WIDTH;
    window->height = WINDOW_HEIGHT;

    window_fetch_position(window);

    window->hooks_length = 0;
    window->hooks = malloc(sizeof(HOOK_T(window)) * window->hooks_length);

    ASSERT(window->hooks, "Couldn't allocate memory!\n");

	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);

    #define DEFAULT_MSG "Logs will be printed here :)"
    window->last_console_msg = malloc(sizeof(char) * strlen(DEFAULT_MSG) + 1);
    window->stats_for_nerds_visible = 0;
    sprintf(window->last_console_msg, "%s", DEFAULT_MSG);
    #undef DEFAULT_MSG

    return window;
}

void fetch_monitor_dimensions(struct Window *window, int *x, int *y) {
    int monitor_index = SDL_GetWindowDisplayIndex(window->window);

    ASSERT(monitor_index >= 0, "Can't fetch the current display index: %s\n",
           SDL_GetError());

    SDL_DisplayMode display_mode;

    ASSERT(SDL_GetDesktopDisplayMode(monitor_index, &display_mode) >= 0,
           "Couldn't get the current display mode: %s\n", SDL_GetError());

    *x = display_mode.w;
    *y = display_mode.h;
}

inline static void resolve_hook(struct Window *window, const char *signal, void *prm) {
    for (size_t i = 0; i < window->hooks_length; ++i) {
        HOOK_T(window) *hook = window->hooks[i];
        if (hook && strcmp(hook->id, signal) == 0) {
            hook->handler(window, hook->meta, prm);
        }
    }
}

void window_refetch_dimensions(struct Window *window) {
    SDL_GetWindowSize(window->window, &window->width, &window->height);

    resolve_hook(window, "window.dimensions::changed", (void*) (&(Vector2d) {
        .x = window->width,
        .y = window->height
    }));
}

void window_toggle_fullscreen(struct Window *window) {
    if (window->is_fullscreen == 0) {
        SDL_SetWindowFullscreen(window->window, SDL_WINDOW_FULLSCREEN);
        fetch_monitor_dimensions(window, &window->width, &window->height);
        window->is_fullscreen = 1;
    } else {
        SDL_SetWindowFullscreen(window->window, 0);
        SDL_SetWindowSize(window->window, WINDOW_WIDTH, WINDOW_HEIGHT);
        window->width = WINDOW_WIDTH;
        window->height = WINDOW_HEIGHT;
        window->is_fullscreen = 0;
    }
}

int calculate_transition_alpha(struct Window *window) {
    int max = window->desired_scene_transition_ticks;
    int min = 0;
    int scaled_value = 0;

    if (window->scene_transition_go_backwards == 0) { // forward
        int left_range =
            max - (window->ticks - window->scene_transition_start_ticks);
        float scale_factor = 255.0 / (max - min);
        scaled_value = 255 - (left_range * scale_factor);
    } else { // backward
        int range = max - min;
        float scale_factor = 255.0 / range;
        scaled_value =
            255 -
            ((window->ticks - window->scene_transition_start_ticks - min) *
             scale_factor);
    }

    if (scaled_value < 0)
        scaled_value = 0;
    else if (scaled_value > 255)
        scaled_value = 255;

    return scaled_value;
}

void apply_scene_switch(struct Window *window) {
    window->current_scene->destroy(window, window->current_scene);
    window->current_scene = window->tmp_scene;
    window->tmp_scene =
        window->current_scene; // hack to don't use NULL cause it gets buggy.
}

void window_scene_transition_splash(struct Window *window) {
    if (window->scene_transition_start_ticks < 0)
        window->scene_transition_start_ticks = window->ticks;

    if (window->desired_scene_transition_ticks < 0)
        window->desired_scene_transition_ticks = 280;

    int transition_alpha = calculate_transition_alpha(window);

    SDL_Rect rect = {0, 0, window->width, window->height};
    SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, transition_alpha);
    SDL_RenderDrawRect(window->renderer, &rect);
    SDL_RenderFillRect(window->renderer, &rect);

    // reset animation to go backwards.
    if (transition_alpha == 255 && window->scene_transition_go_backwards == 0) {
        window->scene_transition_go_backwards = 1;
        window->desired_scene_transition_ticks = -1;
        window->scene_transition_start_ticks = -1;
        apply_scene_switch(window);
    }

    // check if it ended
    if (transition_alpha == 0 && window->scene_transition_go_backwards == 1) {
        window->scene_transition_go_backwards = 0;
        window->desired_scene_transition_ticks = -1;
        window->scene_transition_start_ticks = -1;
        window->render_scene = 1;
    }
}

void window_fetch_mouse_coords(struct Window *window, SDL_Event *ev) {
    // mouse coordinates relatively to the window itself so zero is where the
    // window starts (talking relatively to the screern).
    window->mouse_x = ev->motion.x;
    window->mouse_y = ev->motion.y;

    // these coordinates are being calculated relatively to the desktop itself.
    // so, zero is where the desktop starts.
    window->global_mouse_x = window->mouse_x + window->x;
    window->global_mouse_y = window->mouse_y + window->y;
}

void window_switch_scene(struct Window *window, struct Scene *scene) {
    if (window->render_scene == 0)
        return;
    window->tmp_scene = scene;
    window->render_scene = 0;
}

void window_register_hook(struct Window *window, HOOK_T(window) *hook) {
    #if defined(DEBUG_HOOKS)
        console_log(window, "[WINDOW::DEBUG] Registering hook -> \"%s\" (%p)",
            hook->id, (void*) &hook->handler);
    #endif

    if (sizeof(HOOK_T(window)) * (++window->hooks_length) > sizeof(window->hooks)) {
        window->hooks = realloc(window->hooks, sizeof(HOOK_T(window)) * window->hooks_length);
        ASSERT(window->hooks, "Couldn't reallocate memory!\n");
    }

    window->hooks[window->hooks_length - 1] = hook;
}

// FIXME: See the comment starting at 261-264
void window_unregister_hook(struct Window *window, HOOK_T(window) *hook) {
    size_t hook_index = -1;
    for (size_t i = 0; i < window->hooks_length; ++i) {
        if ((void*) window->hooks[i] == (void*) hook) {
            hook_index = i;
            printf("%zu\n", i);
        }
    }

    ASSERT(hook_index != -1,
        "Error when unregistering hook: Can't find hook with address %p\n",
        (void*) hook);

    char *old_id = malloc(sizeof(char) * strlen(hook->id) + 1);
    sprintf(old_id, "%s", hook->id);

    size_t i = 0;
    size_t j = 0;
    while (i < window->hooks_length) {
        printf("%zu %zu\n", i, j);
        if (i == hook_index) {
            free(window->hooks[i]);
            // still idk what to do here, i tested with this i++
            // and it works with two hooks, but needs more testing as i still
            // think that the j should be incremented too, but there're
            // a few caveats to take in mind before doing it...
            // also, doesn't works if there's only one hook registered
            // im considering in dropping this function, seriously.
            i++;
            continue;
        }
        window->hooks[i] = window->hooks[j];
        i++;
        j++;
    }

    if (sizeof(HOOK_T(window)) * (--window->hooks_length) != sizeof(window->hooks)) {
        window->hooks = realloc(window->hooks, sizeof(HOOK_T(window)) * window->hooks_length);
        ASSERT(window->hooks, "Couldn't reallocate memory!\n");
    }

    #if defined(DEBUG_HOOKS)
        console_log(window, "[WINDOW::DEBUG] Unregistered hook successfully, old id -> %s",
            old_id);
    #endif

    free(old_id);
}

void destroy_window(struct Window *window) {
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    for (size_t i = 0; i < window->hooks_length; ++i) {
        HOOK_T(window) *hook = window->hooks[i];
        #if defined(DEBUG_HOOKS)
            console_log(window, "[WINDOW::DEBUG] Freeing hook -> \"%s\" (%p)", hook->id, (void*) &hook->handler);
        #endif
        free(hook);
    }
    free(window->hooks);
    free(window->last_console_msg);
    free(window);
    Mix_CloseAudio();
    SDL_Quit();
}

Vector2d window_get_mouse_coords_vec2d(struct Window *window) {
    return (Vector2d) {
        .x = (float) window->mouse_x,
        .y = (float) window->mouse_y
    };
}
