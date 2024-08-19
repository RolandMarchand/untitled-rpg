#pragma once

#include <stdio.h>
#include <stdarg.h>

#define ERROR(_fmt, ...) fprintf(stderr, "Error: " _fmt, __VA_ARGS__)

typedef enum {
	ERR_OK = 0,
	ERR_RESOURCE_NOT_FOUND,
	ERR_LOADING_RESOURCES,
} Error;
