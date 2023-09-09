#include <stdlib.h>

#include "vector.h"

Vector2d make_vec2d(float x, float y) {
    return (Vector2d) {
        .x = x,
        .y = y
    };
}

Vector2d *create_vec2d(float x, float y) {
    Vector2d *vec = malloc(sizeof(Vector2d));
    vec->x = x;
    vec->y = y;
    return vec;
}

void sum_vec2d(Vector2d *from, Vector2d *to) {
    to->x += from->x;
    to->y += from->y;
}

void substract_vec2d(Vector2d *from, Vector2d *to) {
    to->x -= from->x;
    to->y -= from->y;
}

void copy_vec2d(Vector2d *from, Vector2d *to) {
    to->x = from->x;
    to->y = from->y;
}
