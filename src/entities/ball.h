#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_rect.h>

#include <time.h>

#include "../entities/bar.h"
#include "../game/window.h"
#include "../audio/audio.h"
#include "../lib/vector/vector.h"
#include "../utils.h"

#pragma once

#define BALL_SOUNDEFFECTS_REBOUND 0
#define BALL_SOUNDEFFECTS_BAR_TOUCH 1
#define BALL_SOUNDEFFECTS_SCORE 2
#define BALL_MAX_SOUNDEFFECTS 3

#define MAX_BALL_BARS 2

#define BALL_DIR_RIGHT "right"
#define BALL_DIR_LEFT "left"
#define BALL_DIR_UP "up"
#define BALL_DIR_DOWN "down"

#define BALL_IS_DIR(prop, dir) strcmp((prop), (dir)) == 0

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

    // soundeffect chunks
    Mix_Chunk **soundeffects;
};

struct Ball *create_ball(struct Window *window, struct Bar **bars);
void ball_render(struct Ball *ball);
void ball_destroy(struct Ball *ball);