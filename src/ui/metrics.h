#include <SDL2/SDL_events.h>

#include "../game/window.h"
#include "../utils.h"

#pragma once

struct Metrics {
    struct Window *window;
    uint16 invisible;
};

struct Metrics *create_metrics(struct Window *window);
void render_metrics(struct Metrics *metrics);
void metrics_handle_events(struct Metrics *metrics, SDL_Event *ev);
void destroy_metrics(struct Metrics *metrics);
