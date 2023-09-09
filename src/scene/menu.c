#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>

#include <math.h>

#include "main.h"
#include "menu.h"
#include "scene.h"

#include "../game/window.h"
#include "../spritesheet/sprites.h"
#include "../texts/text.h"
#include "../audio/audio.h"
#include "../utils.h"

// defines how much should the elements move to the top
#define MOVE_2_TOP 35

#if !defined(MENU_SCENE_UI_INDEXES)
#define MENU_SCENE_UI_INDEXES

#define MENU_SCENE_UI_LOGO 0

#endif

#if !defined(MENU_SCENE_AUDIO_EFFECTS)
#define MENU_SCENE_AUDIO_EFFECTS

#define MENU_SCENE_AUDIO_BACKGROUND 0

#endif

void menu_configure(void *window, struct Scene *scene) {
    CONFIGURE_SCENE(scene);
    scene->ui = malloc(sizeof(struct Sprite*) * 1);
    scene->ui[MENU_SCENE_UI_LOGO] = create_sprite((struct Window*) window, (const char*) "logo");
    scene->audio_effects = malloc(sizeof(struct Mix_Music*));
    scene->audio_effects[MENU_SCENE_AUDIO_BACKGROUND] = load_soundtrack_faded("main-theme");
}

static void render_logo(struct Window *window, struct Scene *scene) {
    struct Sprite *logo = (struct Sprite*) scene->ui[MENU_SCENE_UI_LOGO];

    // forcing size
    logo->surface->w = 624;
    logo->surface->h = 324;

    SDL_Rect dst = {
        .x = (window->width - logo->surface->w) / 2,
        .y = (window->height - logo->surface->h) / 2 - MOVE_2_TOP,
        .w = logo->surface->w,
        .h = logo->surface->h
    };

    SDL_RenderCopy(window->renderer, logo->texture,
        NULL, &dst);
}

static void render_press_start(struct Window *window, struct Scene *scene) {
    struct Sprite *logo = (struct Sprite*) scene->ui[MENU_SCENE_UI_LOGO];

    SDL_Rect dst = {0, 0, 0, 0};

    SDL_Texture *texture = fetch_text_texture((struct TextParams) {
        .text = "Press Enter or click somewhere to begin...",
        .color = (SDL_Color) {0xff, 0xff, 0xff, 0xff},
        .fontpath = "retro",
        .ptsize = 32,
        .window = window,
        .dst = &dst
    });

    // blinking effect.
	const float frec = 0.00001;
	SDL_SetTextureAlphaMod(texture, fabs(sin(frec * (window->ticks * 300)) * 255.0 + 1.0));

    dst.x = (window->width - dst.w) / 2;
    dst.y = ((window->height - dst.h) / 2) + (logo->surface->h / 2) - dst.h - MOVE_2_TOP;

    SDL_RenderCopy(window->renderer, texture,
        NULL, &dst);

    // we don't need it anymore.
    SDL_DestroyTexture(texture);
}

void menu_draw(void *window, struct Scene *scene) {
    render_logo((struct Window*) window, scene);
    render_press_start((struct Window*) window, scene);
}

void menu_handle_events(void *window, struct Scene *scene, SDL_Event *ev) {
    if ((ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT) ||
            (ev->type == SDL_KEYDOWN && ev->key.keysym.sym == SDLK_RETURN)) {
        window_switch_scene((struct Window*) window, create_main_scene());
    }
}

void menu_destroy(void *window, struct Scene *scene) {
    stop_soundtrack_faded((Mix_Music*) scene->audio_effects[MENU_SCENE_AUDIO_BACKGROUND]);
    destroy_sprite((struct Sprite*) scene->ui[MENU_SCENE_UI_LOGO]);
}

struct Scene *create_menu_scene(void) {
    struct Scene *scene = malloc(sizeof(struct Scene));

    scene->configured = 0;
    scene->configure = menu_configure;
    scene->draw = menu_draw;
    scene->handle_events = menu_handle_events;
    scene->destroy = menu_destroy;

    return scene;   
}

#if defined(MOVE_2_TOP)
#    undef MOVE_2_TOP
#endif
