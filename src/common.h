#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#define strdup _strdup
#endif

#if defined(__builtin_expect)
#define unlikely(expr) __builtin_expect(!!(expr), 0)
#define likely(expr) __builtin_expect(!!(expr), 1)
#else
#define unlikely(expr) (expr)
#define likely(expr) (expr)
#endif

#define PRINT_ERR(...) fprintf(stderr, "Error: " __VA_ARGS__)

#define ERROR_TO_STRING(err)                                          \
	((char *[]){                                                  \
		[ERR_OK] = "Operation completed successfully",        \
		[ERR_INVALID_ARGUMENT] = "Invalid argument provided", \
		[ERR_RESOURCE_NOT_FOUND] = "Resource not found",      \
		[ERR_LOADING_RESOURCES] = "Error loading resources",  \
		[ERR_OUT_OF_MEMORY] = "Out of memory",                \
		[ERR_UNKNOWN] = "Unknown error occurred",             \
	}[err])

typedef enum {
	ERR_OK = 0,
	ERR_INVALID_ARGUMENT,
	ERR_RESOURCE_NOT_FOUND,
	ERR_LOADING_RESOURCES,
	ERR_OUT_OF_MEMORY,
	ERR_UNKNOWN,
} Error;
