#pragma once

#include <limits.h>
#include <stdbool.h>
#include "common.h"

#define RESOURCES_ROOT_DIR "/resource"

#define FONT_PATH RESOURCES_ROOT_DIR "/fonts"
#define MODELS_PATH RESOURCES_ROOT_DIR "/models"
#define MUSIC_PATH RESOURCES_ROOT_DIR "/music"
#define SOUND_EFFECTS_PATH RESOURCES_ROOT_DIR "/sound-effects"
#define TEXTURES_PATH RESOURCES_ROOT_DIR "/sprites/textures"
#define ENTITIES_PATH RESOURCES_ROOT_DIR "/sprites/entities"

/* Retrieves the absolute path to RESOURCES_ROOT_DIR.
 *
 * Parameters:
 * - out: A pointer to a buffer where the path will be stored.
 * - capacity: The size of the buffer pointed to by 'out', including space for
 *   the null terminator.
 *
 * Returns:
 * The length of the path written to 'out', excluding the null terminator.
 * If the path cannot be located or if any argument is invalid, the function
 * returns -1.
 */
int GetResourcesPath(char *out, int capacity);
