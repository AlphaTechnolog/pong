#include "../game/window.h"
#include "popup.h"

#pragma once

struct Confirm {
    struct Window *window;
    struct Popup *popup;
    unsigned int popup_width;
    unsigned int popup_height;
    SDL_Color popup_color;
    unsigned int visible;
    const char *message;
    const char *yes_text;
    const char *no_text;
    void (*on_yes)(struct Confirm *confirmation);
    void (*on_cancel)(struct Confirm *confirmation);
};

struct Confirm *create_confirm(struct Confirm confirm_st);

void render_confirm(struct Confirm *confirm);

void toggle_confirm(struct Confirm *confirm);

void raise_confirm(struct Confirm *confirm);

void hide_confirm(struct Confirm *confirm);

void confirm_handle_events(struct Confirm *confirm, SDL_Event *ev);

