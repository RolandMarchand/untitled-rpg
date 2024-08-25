#pragma once

#include "common.h"

#define DICTIONARY_DEFAULT_SIZE 32

typedef struct Entry Entry;

/* String to String implementation of a dictionary/hashmap. This implementation
 * is not thread safe. */
typedef struct {
	/* Dynamic array of Key-value pairs. NULL indicates that the dictionary
	 * has been freed. */
	Entry *entries;
	/* Current count of entries. */
	size_t count;
	/* Total capacity of entries. */
	size_t size;
	/* Random seed for the hashing function. */
	uint32_t seed;
} Dictionary;

/* Initialize a dictionary with the given size. The 'size' parameter specifies
 * the initial number of slots for the dictionary. A size of 0 defaults to
 * DICTIONARY_DEFAULT_SIZE. Return a pointer to the newly created dictionary, or
 * NULL if memory allocation fails. */
Dictionary *DictionaryInit(size_t size);

/* Free the memory allocated for the dictionary. The dictionary must have been
 * previously initialized. */
void DictionaryFree(Dictionary *dict);

/* Retrieve the value associated with the given key from the dictionary. The
 * value is written into the buffer 'out', up to the specified 'capacity'. In
 * case where the value is not found, -1 is returned and 'out' is cleared. The
 * capacity includes space for the null terminator. Return the number of
 * characters written to 'out', not including the null terminator. If the key is
 * not found or if any arguments are invalid, the function returns -1. */
int DictionaryGet(Dictionary *dict, const char *key, char *out, size_t capacity);

/* Insert a new key-value pair into the dictionary, or updates an existing pair
 * if the key is already present. Return an error code if the operation cannot
 * be completed. */
Error DictionarySet(Dictionary *dict, const char *key, const char *value);

/* Remove the key-value pair associated with the specified key from the
 * dictionary. Return an error code if the key is not found or if the operation
 * fails. */
Error DictionaryErase(Dictionary *dict, const char *key);
