#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ball.h"
#include "bar.h"

#include "../game/window.h"
#include "../lib/vector/vector.h"
#include "../config.h"
#include "../utils.h"

static void update_collider(struct Ball *ball) {
    ball->collider = (SDL_Rect) {
        .x = ball->pos.x,
        .y = ball->pos.y,
        .w = ball->size.x,
        .h = ball->size.y
    };
}

static void reset_ball(struct Ball *ball) {
    ball->invisible = 1;
    ball->i_counter = time(0);

    ball->pos = (Vector2d) {
        .x = (ball->window->width - ball->size.x) / 2,
        .y = (ball->window->height - ball->size.y) / 2
    };
}

struct Ball *create_ball(struct Window *window, struct Bar **bars) {
    struct Ball *ball = malloc(sizeof(struct Ball));

    ball->window = window;
    ball->size = (Vector2d) {15.0f, 15.0f};
    ball->bars = bars;
    ball->velocity = 4;

    // invisibility effect.
    ball->i_counter = time(0);
    ball->i_timeout = 1;

    reset_ball(ball);

    ball->dir.horizontal_dir = BALL_DIR_RIGHT;
    ball->dir.vertical_dir = BALL_DIR_UP;

    update_collider(ball);

    return ball;
}

static int check_left_bar_collision(struct Ball *ball, struct Bar *left_bar) {
    return ball->pos.x <= left_bar->pos.x + left_bar->size.x &&
        ball->pos.y >= left_bar->pos.y &&
        ball->pos.y <= left_bar->pos.y + left_bar->size.y;
}

static int check_right_bar_collision(struct Ball *ball, struct Bar *right_bar) {
    return ball->pos.x + ball->size.x >= right_bar->pos.x &&
        ball->pos.y >= right_bar->pos.y &&
        ball->pos.y <= right_bar->pos.y + right_bar->size.y;
}

static int is_at_top_mid(struct Ball *ball, struct Bar *bar) {
    return ball->pos.y <= bar->pos.y + (bar->size.y / 2);
}

static void fill_bars(struct Ball *ball, struct Bar *left_bar, struct Bar *right_bar) {
    for (size_t i = 0; i < MAX_BALL_BARS; ++i) {
        if (IS_BAR_ID(ball->bars[i]->id, BAR_ID_LEFT))
            *left_bar = *ball->bars[i];
        if (IS_BAR_ID(ball->bars[i]->id, BAR_ID_RIGHT))
            *right_bar = *ball->bars[i];
    }
}

#define INVERT_VERT_DIR(ball, bar)                                 \
    (ball)->dir.vertical_dir = is_at_top_mid((ball), (bar))        \
        ? BALL_DIR_UP                                              \
        : BALL_DIR_DOWN;

static void check_rebound(struct Ball *ball) {
    struct Bar left_bar;
    struct Bar right_bar;

    fill_bars(ball, &left_bar, &right_bar);

    if (check_left_bar_collision(ball, &left_bar)) {
        ball->dir.horizontal_dir = BALL_DIR_RIGHT;
        INVERT_VERT_DIR(ball, &left_bar);
    }

    if (check_right_bar_collision(ball, &right_bar)) {
        ball->dir.horizontal_dir = BALL_DIR_LEFT;
        INVERT_VERT_DIR(ball, &right_bar);
    }
}

#undef INVERT_VERT_DIR

static void invert_ball_dir(struct Ball *ball, const char *dir) {
    if (BALL_IS_DIR(dir, BALL_DIR_RIGHT))
        ball->dir.horizontal_dir = BALL_DIR_LEFT;

    if (BALL_IS_DIR(dir, BALL_DIR_LEFT))
        ball->dir.horizontal_dir = BALL_DIR_RIGHT;

    if (BALL_IS_DIR(dir, BALL_DIR_UP))
        ball->dir.vertical_dir = BALL_DIR_DOWN;

    if (BALL_IS_DIR(dir, BALL_DIR_DOWN))
        ball->dir.vertical_dir = BALL_DIR_UP;
}

static void score_to(struct Bar *bar) {
    bar->score++;
    #if defined(DEBUG_BAR_SCORING)
        printf("[BAR::DEBUG] Scoring bar (%p) with id == %s (%f, %f). Score = %llu\n",
            (void*) &bar, bar->id, bar->pos.x, bar->pos.y, bar->score);
    #endif
}

static struct Bar *find_bar(struct Bar **bars, const char *id) {
    for (size_t i = 0; i < MAX_BALL_BARS; ++i) {
        if (IS_BAR_ID(bars[i]->id, id)) {
            return bars[i];
        }
    }

    ERROR("Can't find bar with id -> %s\n", id);

    return NULL;
}

#define AFTER_SCORING(ball)                                     \
    invert_ball_dir((ball), (ball)->dir.horizontal_dir);        \
    invert_ball_dir((ball), (ball)->dir.vertical_dir);          \
    ball->velocity += 0.25f;                                    \
    reset_ball((ball));

static void check_point(struct Ball *ball) {
    if (ball->pos.x <= WINGAP) {
        score_to(find_bar(ball->bars, BAR_ID_RIGHT));
        AFTER_SCORING(ball);
        #if defined(DEBUG_BALL_VELOCITY)
            printf("[BALL::DEBUG] new velocity -> %f\n", ball->velocity);
        #endif
    }

    if (ball->pos.x + ball->size.x >= ball->window->width - WINGAP) {
        score_to(find_bar(ball->bars, BAR_ID_LEFT));
        AFTER_SCORING(ball);
        #if defined(DEBUG_BALL_VELOCITY)
            printf("[BALL::DEBUG] new velocity -> %f\n", ball->velocity);
        #endif
    }
}

#undef AFTER_SCORING

static void check_out_of_bounds(struct Ball *ball) {
    if (ball->pos.y <= 0)
        invert_ball_dir(ball, ball->dir.vertical_dir);

    if (ball->pos.y + ball->size.y >= ball->window->height)
        invert_ball_dir(ball, ball->dir.vertical_dir);

    check_point(ball);
}

static void update_movement(struct Ball *ball) {
    if (BALL_IS_DIR(ball->dir.horizontal_dir, BALL_DIR_RIGHT))
        ball->pos.x += trunc(ball->velocity);

    if (BALL_IS_DIR(ball->dir.horizontal_dir, BALL_DIR_LEFT))
        ball->pos.x -= trunc(ball->velocity);

    if (BALL_IS_DIR(ball->dir.vertical_dir, BALL_DIR_UP))
        ball->pos.y -= trunc(ball->velocity);

    if (BALL_IS_DIR(ball->dir.vertical_dir, BALL_DIR_DOWN))
        ball->pos.y += trunc(ball->velocity);

    check_rebound(ball);
    check_out_of_bounds(ball);
}

void ball_render(struct Ball *ball) {
    // check invisibility
    if (ball->invisible == 1 && time(0) - ball->i_counter > ball->i_timeout)
        ball->invisible = 0;

    if (ball->invisible == 1) return;

    update_collider(ball);
    update_movement(ball);
    SDL_SetRenderDrawColor(ball->window->renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderDrawRect(ball->window->renderer, &ball->collider);
    SDL_RenderFillRect(ball->window->renderer, &ball->collider);
}

void ball_destroy(struct Ball *ball) {
    free(ball->bars);
    free(ball);
}