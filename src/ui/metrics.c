#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "metrics.h"

#include "../game/window.h"
#include "../texts/text.h"
#include "../config.h"
#include "../utils.h"

char *last_console_msg = "";

#define MAX_ACTIONS 2

inline static void render_welcome(uint16 y, struct Metrics *metrics) {
    render_text((struct TextParams) {
        .window = metrics->window,
        .text = "Metrics 4 Nerds | Pong",
        .fontpath = "retro",
        .ptsize = 16,
        .dst = &(SDL_Rect) {WINGAP, y},
        .color = (SDL_Color) {0xff, 0xff, 0xff, 0xff}
    });    
}

inline static void render_fps(uint16 y, struct Metrics *metrics) {
    char *str_fps = int2str(metrics->window->fps);
    char *label = malloc(sizeof(char) * strlen("FPS: ") + strlen(str_fps) + 1);
    sprintf(label, "FPS: %s", str_fps);
    render_text((struct TextParams) {
        .window = metrics->window,
        .text = label,
        .fontpath = "retro",
        .ptsize = 16,
        .dst = &(SDL_Rect) {WINGAP, y},
        .color = (SDL_Color) {0xff, 0xff, 0xff, 0xff}
    });
    free(str_fps);
}

#define ACTION(id, render) \
    {(id), (render)}

static struct {
    const char *id;
    void (*render)(uint16 y, struct Metrics *metrics);
} actions[MAX_ACTIONS] = {
    ACTION("welcome", render_welcome),
    ACTION("fps", render_fps)
};

#undef ACTION

inline static int stop(void) {
    #ifdef DEBUG_ENABLE_METRICS
        return 0;
    #else
        return 1;
    #endif
}

#define STOP_NULL() if (stop()) return NULL;
#define STOP() if (stop()) return;

struct Metrics *create_metrics(struct Window *window) {
    STOP_NULL();

    struct Metrics *metrics = malloc(sizeof(struct Metrics));

    metrics->window = window;
    metrics->invisible = 1;

    return metrics;
}

void render_metrics(struct Metrics *metrics) {
    STOP();

    if (metrics->invisible == 1) return;

    for (size_t i = 0; i < MAX_ACTIONS; ++i) {
        __typeof__(actions[0]) action = actions[i];
        action.render(WINGAP * i * (i > 0 ? 2 : 1), metrics);
    }
}

void metrics_handle_events(struct Metrics *metrics, SDL_Event *ev) {
    if (ev->key.keysym.sym == SDLK_F3 && ev->type == SDL_KEYDOWN) {
        metrics->invisible = !metrics->invisible;
    }
}

void destroy_metrics(struct Metrics *metrics) {
    STOP();
    free(metrics);
}

#undef STOP
#undef STOP_NULL
#undef MAX_ACTIONS
