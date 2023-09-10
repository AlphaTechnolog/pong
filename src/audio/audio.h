#include <SDL2/SDL_mixer.h>

#pragma once

// soundtrack (long musics)
Mix_Music *load_soundtrack_faded(const char *filename);
void stop_soundtrack_faded(Mix_Music *music);
Mix_Chunk *load_soundeffect(const char *filename);
void start_soundeffect(Mix_Chunk *chunk);
void stop_soundeffect(Mix_Chunk *chunk);
