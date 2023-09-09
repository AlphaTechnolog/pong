#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

#include <stdio.h>
#include <stdlib.h>

#include "confirm.h"
#include "popup.h"

#include "../config.h"
#include "../game/window.h"
#include "../lib/vector/vector.h"
#include "../texts/text.h"

const unsigned int button_height = 30;

void confirm_update_popup(struct Confirm *confirm) {
    Vector2d *confirm_size =
        create_vec2d(confirm->popup_width, confirm->popup_height);

    Vector2d *confirm_pos =
        create_vec2d((confirm->window->width - confirm_size->x) / 2,
                     (confirm->window->height - confirm_size->y) / 2);

    confirm->popup = create_popup(confirm->window, confirm_pos, confirm_size,
                                  confirm->popup_color);
}

struct Confirm *create_confirm(struct Confirm confirm_st) {
    struct Confirm *confirm = malloc(sizeof(struct Confirm));

    confirm->window = confirm_st.window;
    confirm->visible = confirm_st.visible;
    confirm->message = confirm_st.message;
    confirm->yes_text = confirm_st.yes_text;
    confirm->no_text = confirm_st.no_text;
    confirm->on_yes = confirm_st.on_yes;
    confirm->on_cancel = confirm_st.on_cancel;

    confirm->popup_width = confirm_st.popup_width;
    confirm->popup_height = confirm_st.popup_height;
    confirm->popup_color = confirm_st.popup_color;

    confirm_update_popup(confirm);

    return confirm;
}

SDL_Texture *confirm_button_text(struct Window *window, const char *text,
                                 SDL_Rect *dst) {
    return fetch_text_texture(
        (struct TextParams){.window = window,
                            .fontpath = "retro",
                            .text = (char *)text,
                            .ptsize = 12,
                            .color = (SDL_Color){0xff, 0xff, 0xff, 0xff},
                            .dst = dst});
}

// clang-format off

Vector2d calculate_yes_button_position(struct Confirm *confirm) {
    return (Vector2d) {
        .x = confirm->popup->coordinates->x + WINGAP,
        .y = (confirm->popup->coordinates->y + confirm->popup->size->y) - WINGAP - button_height
    };
}

Vector2d calculate_no_button_position(struct Confirm *confirm) {
    return (Vector2d) {
        .x = confirm->popup->coordinates->x + ((int)confirm->popup->size->x / 2) + ((int)WINGAP / 2),
        .y = (confirm->popup->coordinates->y + confirm->popup->size->y) - WINGAP - button_height,
    };
}


void render_confirm_buttons(struct Confirm *confirm) {
    Vector2d yes_button_position = calculate_yes_button_position(confirm);
    Vector2d no_button_position = calculate_no_button_position(confirm);

    SDL_Rect yes_button = (SDL_Rect) {
        .x = yes_button_position.x,
        .y = yes_button_position.y,
        .w = ((int)confirm->popup->size->x / 2) - WINGAP,
        .h = button_height
    };

    SDL_SetRenderDrawColor(confirm->window->renderer, 0x87, 0xdc, 0x76,
                           0xff); // green

    SDL_RenderDrawRect(confirm->window->renderer, &yes_button);
    SDL_RenderFillRect(confirm->window->renderer, &yes_button);

    SDL_Rect no_button = (SDL_Rect) {
        .x = no_button_position.x,
        .y = no_button_position.y,
        .w = ((int)confirm->popup->size->x / 2) - ((int)WINGAP / 2) - WINGAP,
        .h = button_height
    };

    SDL_SetRenderDrawColor(confirm->window->renderer, 0xf0, 0x4a, 0x73,
                           0xff); // red

    SDL_RenderDrawRect(confirm->window->renderer, &no_button);
    SDL_RenderFillRect(confirm->window->renderer, &no_button);

    SDL_Rect dst = {0, 0, 0, 0};

    SDL_Texture *yes_texture =
        confirm_button_text(confirm->window, confirm->yes_text, &dst);

    dst.x = yes_button.x + ((yes_button.w - dst.w) / 2);
    dst.y = yes_button.y + ((yes_button.h - dst.h) / 2);

    render_text_texture(confirm->window, yes_texture, dst);

    SDL_Texture *no_texture =
        confirm_button_text(confirm->window, confirm->no_text, &dst);

    dst.x = no_button.x + ((no_button.w - dst.w) / 2);
    dst.y = no_button.y + ((no_button.h - dst.h) / 2);

    render_text_texture(confirm->window, no_texture, dst);
}

// clang-format on

void render_confirm_layout(struct Confirm *confirm) {
    SDL_Rect dst = {0, 0, 0, 0};

    SDL_Texture *message_text = fetch_text_texture(
        (struct TextParams){.window = confirm->window,
                            .fontpath = "retro",
                            .text = confirm->message,
                            .ptsize = 18,
                            .color = (SDL_Color){0xff, 0xff, 0xff, 0xff},
                            .dst = &dst});

    dst.x = confirm->popup->coordinates->x +
            ((confirm->popup->size->x - dst.w) / 2);
    dst.y = confirm->popup->coordinates->y + WINGAP;

    render_text_texture(confirm->window, message_text, dst);
    render_confirm_buttons(confirm);
}

void render_confirm(struct Confirm *confirm) {
    if (confirm->visible == 0)
        return;
    confirm_update_popup(confirm);
    render_popup(confirm->popup);
    render_confirm_layout(confirm);
}

#if !defined(CONFIRM_HELPERS)
#define CONFIRM_HELPERS

#define DEFCONTROLFN(name, value)                                              \
    void name(struct Confirm *confirm) { confirm->visible = (value); }

#endif

DEFCONTROLFN(raise_confirm, 1)
DEFCONTROLFN(hide_confirm, 0)

void toggle_confirm(struct Confirm *confirm) {
    if (confirm->visible == 0)
        raise_confirm(confirm);
    else if (confirm->visible == 1)
        hide_confirm(confirm);
}

void confirm_handle_events(struct Confirm *confirm, SDL_Event *ev) {
    if (ev->type == SDL_MOUSEBUTTONDOWN &&
        ev->button.button == SDL_BUTTON_LEFT && confirm->visible == 1) {
        Vector2d yes_button_position = calculate_yes_button_position(confirm);
        Vector2d no_button_position = calculate_no_button_position(confirm);

        // executes the callback if the mouse click is inside the button.
        if (confirm->window->mouse_x >= yes_button_position.x &&
            confirm->window->mouse_x <= yes_button_position.x +
                                            ((int)confirm->popup->size->x / 2) -
                                            WINGAP &&
            confirm->window->mouse_y >= yes_button_position.y &&
            confirm->window->mouse_y <= yes_button_position.y + button_height) {
            confirm->on_yes(confirm);
        }

        // executes the callback if the mouse click is inside the button.
        if (confirm->window->mouse_x >= no_button_position.x &&
            confirm->window->mouse_x <= no_button_position.x +
                                            ((int)confirm->popup->size->x / 2) -
                                            ((int)WINGAP / 2) - WINGAP &&
            confirm->window->mouse_y >= no_button_position.y &&
            confirm->window->mouse_y <= no_button_position.y + button_height) {
            confirm->on_cancel(confirm);
        }
    }
}

