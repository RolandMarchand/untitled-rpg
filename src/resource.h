#pragma once

#include <limits.h>

#define RESOURCE_PATH "/tmp/untitled_game"

#define FONT_PATH RESOURCE_PATH "/fonts"
#define MODELS_PATH RESOURCE_PATH "/models"
#define MUSIC_PATH RESOURCE_PATH "/music"
#define SOUND_EFFECTS_PATH RESOURCE_PATH "/sound-effects"
#define TEXTURES_PATH RESOURCE_PATH "/sprites/textures"
#define ENTITIES_PATH RESOURCE_PATH "/sprites/entities"

typedef enum {
	ERR_OK = 0,
	ERR_RESOURCE_NOT_FOUND,
	ERR_LOADING_RESOURCES,
} ResourceError;

typedef struct {
	ResourceError error;
	char path[PATH_MAX];
} Resource;

Resource LoadResource();
