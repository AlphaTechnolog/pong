#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "audio.h"
#include "../utils.h"

static char *soundtrack_resolve_filename(const char *partial_filename) {
	const unsigned int length = 1
		+ strlen("./music/soundtracks/")
		+ strlen(partial_filename)
		+ strlen(".mp3");

	char *result = malloc(sizeof(char) * length);
	ASSERT(result, "Cannot allocate memory!\n");
	sprintf(result, "./music/soundtracks/%s.mp3", partial_filename);
	return result;
}

Mix_Music *load_soundtrack_faded(const char *filename) {
	char *file = soundtrack_resolve_filename(filename);

	Mix_Music *music = Mix_LoadMUS(file);
	Mix_FadeInMusic(music, -1, 3200);
	free(file);

	return music;
}

void stop_soundtrack_faded(Mix_Music *music) {
	Mix_FadeOutMusic(500);
	SDL_Delay(500);
	Mix_FreeMusic(music);
}

static char *soundeffect_resolve_filename(const char *partial_filename) {
	const unsigned int length = 1
		+ strlen("./music/soundeffects/")
		+ strlen(partial_filename)
		+ strlen(".wav");

	char *result = malloc(sizeof(char) * length);
	ASSERT(result, "Cannot allocate memory!\n");
	sprintf(result, "./music/soundeffects/%s.wav", partial_filename);
	return result;
}

Mix_Chunk *load_soundeffect(const char *filename) {
	char *file = soundeffect_resolve_filename(filename);
	Mix_Chunk *chunk = Mix_LoadWAV(file);
	free(file);
	return chunk;
}

void start_soundeffect(Mix_Chunk *chunk) {
	ASSERT(Mix_PlayChannel(-1, chunk, 0) >= 0, "Can't play sound effect\n");
}

void stop_soundeffect(Mix_Chunk *chunk) {
	Mix_FreeChunk(chunk);
}