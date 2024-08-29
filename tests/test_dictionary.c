#include "common.h"
#include "dictionary.h"

#define MAX 256
#define LARGE_DATASET_SIZE 1000000
#define ASSERT(test, ...)				\
	do {						\
	if (!(test)) {					\
		fprintf(stderr, "%d: ", __LINE__);	\
		PRINT_ERR(__VA_ARGS__);			\
		succeed = -1;				\
	}						\
	} while (0)

int succeed = 0;

static void test_initialization()
{
	
	Dictionary *dict = DictionaryInit(0);

	ASSERT(dict != NULL, "Unable to instance a dictionary.\n");

	/* Test initializing with default size. */
	ASSERT(dict->size == DICTIONARY_DEFAULT_SIZE,
	       "Wrong default size for new dictionaries. Expected %d, got %lu.\n",
	       DICTIONARY_DEFAULT_SIZE, dict->size);
	DictionaryFree(dict);

	for (int i = 0; i < 1000; i++) {
		size_t size = rand() % LARGE_DATASET_SIZE;
		dict = DictionaryInit(size);
		ASSERT(dict->size == size,
		       "Wrong default size for new dictionaries. Expected %d, got %lu.\n",
		       DICTIONARY_DEFAULT_SIZE, dict->size);
		DictionaryFree(dict);
	}
}

static void test_insertion()
{
	
	char buf[MAX];
	Dictionary *dict = DictionaryInit(0);

	ASSERT(dict != NULL, "Unable to instance a dictionary.\n");

	Error err = DictionarySet(dict, "key1", "value1");
	ASSERT(err == ERR_OK,
	       "Error %s upon setting a dictionary entry.\n",
	       ERROR_TO_STRING(err));

	ASSERT(dict->count == 1,
	       "Wrong count set, expected 1, got %lu.\n", dict->count);

	int written = DictionaryGet(dict, "key1", buf, MAX);
	ASSERT(written == strlen("value1"),
	       "Unable to fill buffer with value. Expected %ld writes, got %d writes.\n",
	       strlen("value1"), written);

	ASSERT(strcmp(buf, "value1") == 0,
	       "Wrong value read. Expected 'value1', got '%s'.\n", buf);

	DictionaryFree(dict);
}

static void test_large_dataset()
{
	
	Dictionary *dict = DictionaryInit(0);
	char key[32];
	char value[32];

	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "key%zu", i);
		snprintf(value, sizeof(value), "value%zu", i);

		Error err = DictionarySet(dict, key, value);
		ASSERT(err == ERR_OK,
		       "Error %s upon setting a large dataset entry %zu.\n",
		       ERROR_TO_STRING(err), i);
	}

	ASSERT(dict->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %lu.\n",
	       LARGE_DATASET_SIZE, dict->count);

	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "key%zu", i);
		snprintf(value, sizeof(value), "value%zu", i);

		char buf[MAX];
		int written = DictionaryGet(dict, key, buf, MAX);
		ASSERT(written == strlen(value),
		       "Unable to fill buffer with value. Expected %ld writes, got %d writes.\n",
		       strlen(value), written);
		ASSERT(strcmp(buf, value) == 0,
		       "Wrong value read for key %zu. Expected '%s', got '%s'.\n", i, value, buf);
	}

	DictionaryFree(dict);
}

static void test_deletion()
{
	
	Dictionary *dict = DictionaryInit(0);

	ASSERT(dict != NULL, "Unable to instance a dictionary.\n");

	Error err = DictionarySet(dict, "key1", "value1");
	ASSERT(err == ERR_OK, "Error %s upon setting a dictionary entry.\n",
	       ERROR_TO_STRING(err));


	err = DictionaryErase(dict, "key1");
	ASSERT(err == ERR_OK, "Error %s upon erasing a dictionary entry.\n",
	       ERROR_TO_STRING(err));

	ASSERT(dict->count == 0, "Wrong count set, expected 0, got %lu.\n",
	       dict->count);

	DictionaryFree(dict);
}

static void test_stress_deletion()
{
	
	Dictionary *dict = DictionaryInit(0);
	char key[32];
	char value[32];

	/* Insert a large number of entries */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "key%zu", i);
		snprintf(value, sizeof(value), "value%zu", i);
		int err = DictionarySet(dict, key, value);
		ASSERT(err == ERR_OK, "%s upon setting a dictionary entry.\n",
		       ERROR_TO_STRING(err));
		ASSERT(dict->count == i + 1,
		       "expected the count to be of %lu, got %lu\n",
		       i + 1, dict->count);
	}

	/* Delete every other entry */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i += 2) {
		snprintf(key, sizeof(key), "key%zu", i);
		Error err = DictionaryErase(dict, key);
		ASSERT(err == ERR_OK, "%s upon erasing a dictionary entry.\n",
		       ERROR_TO_STRING(err));
		size_t expected = LARGE_DATASET_SIZE - (i + 2) / 2;
		ASSERT(dict->count == expected,
		       "expected the count to be of %lu, got %lu\n",
		       expected, dict->count);
	}

	ASSERT(dict->count == LARGE_DATASET_SIZE / 2,
	       "Wrong count after stress deletion, expected %d, got %lu.\n",
	       LARGE_DATASET_SIZE / 2, dict->count);

	/* Verify remaining entries */
	for (size_t i = 1; i < LARGE_DATASET_SIZE; i += 2) {
		snprintf(key, sizeof(key), "key%zu", i);
		snprintf(value, sizeof(value), "value%zu", i);

		char buf[MAX];
		int written = DictionaryGet(dict, key, buf, MAX);
		ASSERT(written == strlen(value),
		       "Unable to fill buffer with value. Expected %ld writes, got %d writes.\n",
		       strlen(value), written);
		ASSERT(strcmp(buf, value) == 0,
		       "Wrong value read for key %zu. Expected '%s', got '%s'.\n",
		       i, value, buf);
	}

	DictionaryFree(dict);
}

static int compare_strings(const void *a, const void *b)
{
	const char *str1 = *(char **)a;
	const char *str2 = *(char **)b;
	return strcmp(str1, str2);
}

static void test_getting_all_keys()
{
	Dictionary *dict = DictionaryInit(0);
	char key[32];
	char value[32];

	/* Fill dictionary with input. */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "key%zu", i);
		snprintf(value, sizeof(value), "value%zu", i);

		Error err = DictionarySet(dict, key, value);
		ASSERT(err == ERR_OK, "Error %s upon setting a large dataset entry %zu.\n",
			ERROR_TO_STRING(err), i);
	}

	ASSERT(dict->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %lu.\n",
	       LARGE_DATASET_SIZE, dict->count);

	/* Get all keys. */
	size_t capacity = dict->count * sizeof(char*);
	char **keys = malloc(capacity);
	Error err = DictionaryGetKeys(dict, keys, capacity);
	ASSERT(err == ERR_OK, "%s: unable to get all keys in test\n",
	       ERROR_TO_STRING(err));

	/* Sort to easily compare input with output. */
	qsort(keys, dict->count, sizeof(char*), compare_strings);

	for (size_t i = 0; i < dict->count; i++) {
		snprintf(key, sizeof(key), "key%zu", i);

		ASSERT(strcmp(key, keys[i]) == 0,
		       "Input and output don't match when getting all keys.\n");
	}

	free(keys);
	DictionaryFree(dict);
}

int main(void)
{
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
