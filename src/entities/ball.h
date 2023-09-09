#include <SDL2/SDL_rect.h>

#include <time.h>

#include "../entities/bar.h"
#include "../game/window.h"
#include "../lib/vector/vector.h"
#include "../utils.h"

#pragma once

#define BALL_DIR_RIGHT "right"
#define BALL_DIR_LEFT "left"
#define BALL_DIR_UP "up"
#define BALL_DIR_DOWN "down"

#define BALL_IS_DIR(prop, dir) strcmp((prop), (dir)) == 0

#define MAX_BALL_BARS 2

struct Ball {
    Vector2d pos;
    Vector2d size;
    SDL_Rect collider;
    uint8 invisible;
    float velocity;

    // invisibility effect timers.
    time_t i_counter;
    time_t i_timeout;

    struct Window *window;
    struct Bar **bars;

    struct {
        char *vertical_dir;
        char *horizontal_dir;
    } dir;
};

struct Ball *create_ball(struct Window *window, struct Bar **bars);
void ball_render(struct Ball *ball);
void ball_destroy(struct Ball *ball);