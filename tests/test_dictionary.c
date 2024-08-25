#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "dictionary.h"

#define MAX 256
#define LARGE_DATASET_SIZE 1000000
#define ASSERT(...)					\
	do {						\
		fprintf(stderr, "%d: ", __LINE__);	\
		ERROR(__VA_ARGS__);			\
		succeed = -1;				\
	} while (0)

int succeed = 0;

static void test_initialization() {
	
	Dictionary *dict = DictionaryInit(0);

	if (dict == NULL) {
		ASSERT("Unable to instance a dictionary.\n");
	}

	/* Test initializing with default size. */
	if (dict->size != DICTIONARY_DEFAULT_SIZE) {
		ASSERT("Wrong default size for new dictionaries. Expected %d, got %lu.\n",
		       DICTIONARY_DEFAULT_SIZE, dict->size);
	}
	DictionaryFree(dict);

	for (int i = 0; i < 1000; i++) {
		size_t size = rand() % LARGE_DATASET_SIZE;
		dict = DictionaryInit(size);
		if (dict->size != size) {
			ASSERT("Wrong default size for new dictionaries. Expected %d, got %lu.\n",
			       DICTIONARY_DEFAULT_SIZE, dict->size);
		}
		DictionaryFree(dict);
	}
}

static void test_insertion() {
	
	char buf[MAX];
	Dictionary *dict = DictionaryInit(0);

	if (dict == NULL) {
		ASSERT("Unable to instance a dictionary.\n");
	}

	Error err = DictionarySet(dict, "key1", "value1");
	if (err != ERR_OK) {
		ASSERT("Error %s upon setting a dictionary entry.\n",
		      ERROR_TO_STRING(err));
	}
	if (dict->count != 1) {
		ASSERT("Wrong count set, expected 1, got %lu.\n", dict->count);
	}

	int written = DictionaryGet(dict, "key1", buf, MAX);
	if (written != strlen("value1")) {
		ASSERT("Unable to fill buffer with value. Expected %ld writes, got %d writes.\n",
		       strlen("value1"), written);
	}
	if (strcmp(buf, "value1") != 0) {
		ASSERT("Wrong value read. Expected 'value1', got '%s'.\n", buf);
	}

	DictionaryFree(dict);
}

static void test_large_dataset() {
	
	Dictionary *dict = DictionaryInit(0);
	char key[32];
	char value[32];

	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "key%zu", i);
		snprintf(value, sizeof(value), "value%zu", i);

		Error err = DictionarySet(dict, key, value);
		if (err != ERR_OK) {
			ASSERT("Error %s upon setting a large dataset entry %zu.\n",
			       ERROR_TO_STRING(err), i);
		}
	}

	if (dict->count != LARGE_DATASET_SIZE) {
		ASSERT("Wrong count set for large dataset, expected %d, got %lu.\n", LARGE_DATASET_SIZE, dict->count);
	}

	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "key%zu", i);
		snprintf(value, sizeof(value), "value%zu", i);

		char buf[MAX];
		int written = DictionaryGet(dict, key, buf, MAX);
		if (written != strlen(value)) {
			ASSERT("Unable to fill buffer with value. Expected %ld writes, got %d writes.\n",
			       strlen(value), written);
		}
		if (strcmp(buf, value) != 0) {
			ASSERT("Wrong value read for key %zu. Expected '%s', got '%s'.\n", i, value, buf);
		}
	}

	DictionaryFree(dict);
}

static void test_deletion() {
	
	Dictionary *dict = DictionaryInit(0);

	if (dict == NULL) {
		ASSERT("Unable to instance a dictionary.\n");
	}

	Error err = DictionarySet(dict, "key1", "value1");
	if (err != ERR_OK) {
		ASSERT("Error %s upon setting a dictionary entry.\n",
		      ERROR_TO_STRING(err));
	}

	err = DictionaryErase(dict, "key1");
	if (err != ERR_OK) {
		ASSERT("Error %s upon erasing a dictionary entry.\n",
		      ERROR_TO_STRING(err));		
	}
	if (dict->count != 0) {
		ASSERT("Wrong count set, expected 0, got %lu.\n", dict->count);
	}

	DictionaryFree(dict);
}

static void test_stress_deletion() {
	
	Dictionary *dict = DictionaryInit(LARGE_DATASET_SIZE);
	char key[32];
	char value[32];

	/* Insert a large number of entries */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "key%zu", i);
		snprintf(value, sizeof(value), "value%zu", i);
		int err = DictionarySet(dict, key, value);
		if (err != ERR_OK) {
			ASSERT("%s upon setting a dictionary entry.\n",
			       ERROR_TO_STRING(err));
		}
		if (dict->count != i + 1) {
			ASSERT("expected the count to be of %lu, got %lu\n",
			       i + 1, dict->count);
		}
	}

	/* Delete every other entry */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i += 2) {
		snprintf(key, sizeof(key), "key%zu", i);
		Error err = DictionaryErase(dict, key);
		if (err != ERR_OK) {
			ASSERT("%s upon erasing a dictionary entry.\n",
			       ERROR_TO_STRING(err));
		}
		size_t expected = LARGE_DATASET_SIZE - (i + 2) / 2;
		if (dict->count != expected) {
			ASSERT("expected the count to be of %lu, got %lu\n",
			       expected, dict->count);
		}
	}

	if (dict->count != LARGE_DATASET_SIZE / 2) {
		ASSERT("Wrong count after stress deletion, expected %d, got %lu.\n", LARGE_DATASET_SIZE / 2, dict->count);
	}

	/* Verify remaining entries */
	for (size_t i = 1; i < LARGE_DATASET_SIZE; i += 2) {
		snprintf(key, sizeof(key), "key%zu", i);
		snprintf(value, sizeof(value), "value%zu", i);

		char buf[MAX];
		int written = DictionaryGet(dict, key, buf, MAX);
		if (written != strlen(value)) {
			ASSERT("Unable to fill buffer with value. Expected %ld writes, got %d writes.\n",
			       strlen(value), written);
		}
		if (strcmp(buf, value) != 0) {
			ASSERT("Wrong value read for key %zu. Expected '%s', got '%s'.\n", i, value, buf);
		}
	}

	DictionaryFree(dict);
}

int main(void) {
	test_initialization();
	test_initialization();
	test_initialization();
	test_insertion();
	test_insertion();
	test_insertion();
	test_large_dataset();
	test_large_dataset();
	test_large_dataset();
	test_deletion();
	test_deletion();
	test_deletion();
	test_stress_deletion();
	test_stress_deletion();
	test_stress_deletion();

	return succeed;
}
