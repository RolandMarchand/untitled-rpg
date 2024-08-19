#pragma once

#include <limits.h>

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
