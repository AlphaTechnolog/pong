#include <SDL2/SDL_mixer.h>

#pragma once

// soundtrack (long musics)
Mix_Music *load_soundtrack_faded(const char *filename);
void stop_soundtrack_faded(Mix_Music *music);
