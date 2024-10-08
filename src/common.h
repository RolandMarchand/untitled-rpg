#pragma once

#ifdef _MSC_VER
#define strdup _strdup
#define ALIGN(x) __declspec(align(x))
#else
#define ALIGN(x) __attribute__((aligned(x)))
#endif

#if defined(__builtin_expect)
#define unlikely(expr) __builtin_expect(!!(expr), 0)
#define likely(expr) __builtin_expect(!!(expr), 1)
#else
#define unlikely(expr) (expr)
#define likely(expr) (expr)
#endif

#define PRINT_ERR(...) fprintf(stderr, "Error: " __VA_ARGS__)

#define ERROR_TO_STRING(err)                                                   \
	(((err) >= 0 && (err) < ERR_UNKNOWN) ?                                 \
		 (const char *[]){                                             \
			 [ERR_OK] = "Operation completed successfully",        \
			 [ERR_FILE_NOT_FOUND] = "File not found",              \
			 [ERR_FILE_READ_FAILURE] =                             \
				 "Failed to read from the file",               \
			 [ERR_FILE_SEEK_FAILURE] =                             \
				 "Failed to move the file pointer",            \
			 [ERR_FILE_WRITE_FAILURE] =                            \
				 "Failed to write to the file",                \
			 [ERR_INSUFFICIENT_SPACE] =                            \
				 "Insufficient space available",               \
			 [ERR_INVALID_ARGUMENT] = "Invalid argument provided", \
			 [ERR_LOADING_RESOURCES] = "Error loading resources",  \
			 [ERR_NULL_REFERENCE] = "Null reference encountered",  \
			 [ERR_OUT_OF_MEMORY] = "Out of memory",                \
			 [ERR_RESOURCE_NOT_FOUND] = "Resource not found",      \
			 [ERR_INVALID_SYNTAX] = "Syntax error",                  \
			 [ERR_UNKNOWN] = "Unknown error occurred",             \
		 }[err] :                                                      \
		 "Invalid error code")

typedef enum {
	ERR_OK = 0,
	ERR_FILE_NOT_FOUND,
	ERR_FILE_READ_FAILURE,
	ERR_FILE_SEEK_FAILURE,
	ERR_FILE_WRITE_FAILURE,
	ERR_INSUFFICIENT_SPACE,
	ERR_INVALID_ARGUMENT,
	ERR_LOADING_RESOURCES,
	ERR_NULL_REFERENCE,
	ERR_OUT_OF_MEMORY,
	ERR_RESOURCE_NOT_FOUND,
	ERR_INVALID_SYNTAX,

	ERR_UNKNOWN, /* Unknown should be the last error in the enum */
} Error;
