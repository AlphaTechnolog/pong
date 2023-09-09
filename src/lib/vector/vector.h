// this module provides support for a simple vector (2d) implementation
// looking for a simple maintainance and simple function-based usage.

#pragma once

typedef struct vector2d {
    float x;
    float y;
} Vector2d;

Vector2d make_vec2d(float x, float y);

Vector2d *create_vec2d(float x, float y);

void sum_vec2d(Vector2d *from, Vector2d *to);

void substract_vec2d(Vector2d *from, Vector2d *to);

void copy_vec2d(Vector2d *from, Vector2d *to);
