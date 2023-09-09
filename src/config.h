#pragma once

// windows configuration
#define WINDOW_TITLE "Simple Pong"
#define WINDOW_WIDTH 1020
#define WINDOW_HEIGHT 820

// general behavior
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)
#define FULLSCREEN_BY_DEFAULT
#define DEFAULT_BALL_ACC 10

// ui configuration
#define WINGAP 12

// debugging configuration
// #define DEBUG_HOOKS
// #define DEBUG_BAR_SCORING
// #define DEBUG_BALL_VELOCITY

// draw colliders for bars? (players)
// #define RENDER_COLLIDERS_BOXES
