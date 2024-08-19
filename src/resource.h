#pragma once

#include <limits.h>
#include <stdbool.h>
#include "common.h"

#define RESOURCE_PATH "/tmp/untitled_game"

#define FONT_PATH RESOURCE_PATH "/fonts"
#define MODELS_PATH RESOURCE_PATH "/models"
#define MUSIC_PATH RESOURCE_PATH "/music"
#define SOUND_EFFECTS_PATH RESOURCE_PATH "/sound-effects"
#define TEXTURES_PATH RESOURCE_PATH "/sprites/textures"
#define ENTITIES_PATH RESOURCE_PATH "/sprites/entities"

Error LoadResource();
bool IsResourceLoaded();
