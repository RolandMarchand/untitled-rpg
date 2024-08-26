#pragma once

#include <limits.h>
#include <stdbool.h>
#include "common.h"

#define ENTITIES_PATH "/sprites/entities"
#define FONT_PATH "/fonts"
#define MODELS_PATH "/models"
#define MUSIC_PATH "/music"
#define SOUND_EFFECTS_PATH "/sound-effects"
#define TEXTURES_PATH "/sprites/textures"

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
int GetResourcesPath(char *out, size_t capacity);
