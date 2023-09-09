#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#pragma once

#define ASSERT(condition, message, ...) \
	if (!(condition)) { \
		printf(message, ##__VA_ARGS__); \
		exit(EXIT_FAILURE); \
	}

#define CHECK_FOR_FILE(file) \
	ASSERT(access(file, F_OK) == 0, "Cannot open %s: No such file or directory\n", \
		file);

#define CONFIGURE_SCENE(scene) \
	if ((scene)->configured == 1) { \
		return; \
	} \
	scene->configured = 1;

// types for readability
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;
typedef unsigned long long uint128;
typedef short int16;
typedef int int32;
typedef long int64;
typedef long long int128;

int int_bytes(int n);
char *int2str(int n);

