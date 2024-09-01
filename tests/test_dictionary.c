#include "common.h"
#include "dictionary.h"

#include <stdio.h>
#include <string.h>

enum {
	MAX = 256,
	LARGE_DATASET_SIZE = 50000,
};

#define ASSERT(test, ...)				\
	do {						\
	if (!(test)) {					\
		fprintf(stderr, "%d: ", __LINE__);	\
		PRINT_ERR(__VA_ARGS__);			\
		succeed = -1;				\
	}						\
	} while (0)

int succeed = 0;

static void TestInitialization()
{
	
	Dictionary *dict = DictionaryInit(0);

	ASSERT(dict != NULL, "Unable to instance a dictionary.\n");

	/* Test initializing with default size. */
	ASSERT(dict->capacity == DICTIONARY_DEFAULT_CAPACITY,
	       "Wrong default size for new dictionaries. Expected %d, got %zu.\n",
	       DICTIONARY_DEFAULT_CAPACITY, dict->capacity);
	DictionaryFree(dict);

	for (int i = 0; i < 1000; i++) {
		size_t size = rand() % LARGE_DATASET_SIZE;
		dict = DictionaryInit(size);
		ASSERT(dict->capacity == size,
		       "Wrong default size for new dictionaries. Expected %d, got %zu.\n",
		       DICTIONARY_DEFAULT_CAPACITY, dict->capacity);
		DictionaryFree(dict);
	}
}

static void TestInsertion()
{
	char buf[MAX];
	Dictionary *dict = DictionaryInit(0);

	ASSERT(dict != NULL, "Unable to instance a dictionary.\n");

	Error err = DictionarySet(dict, "key1", "value1");
	ASSERT(err == ERR_OK,
	       "Error %s upon setting a dictionary entry.\n",
	       ERROR_TO_STRING(err));

	ASSERT(dict->count == 1,
	       "Wrong count set, expected 1, got %zu.\n", dict->count);

	int written = DictionaryGet(dict, "key1", buf, MAX);
	ASSERT(written == strlen("value1"),
	       "Unable to fill buffer with value. Expected %zd writes, got %d writes.\n",
	       strlen("value1"), written);

	ASSERT(strcmp(buf, "value1") == 0,
	       "Wrong value read. Expected 'value1', got '%s'.\n", buf);

	DictionaryFree(dict);
}

static void TestLargeDataset()
{
	
	Dictionary *dict = DictionaryInit(0);
	char key[32];
	char value[32];

	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "%zukey", i);
		snprintf(value, sizeof(value), "%zuvalue", i);

		Error err = DictionarySet(dict, key, value);
		ASSERT(err == ERR_OK,
		       "Error %s upon setting a large dataset entry %zu.\n",
		       ERROR_TO_STRING(err), i);
	}

	ASSERT(dict->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %zu.\n",
	       LARGE_DATASET_SIZE, dict->count);

	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "%zukey", i);
		snprintf(value, sizeof(value), "%zuvalue", i);

		char buf[MAX];
		int written = DictionaryGet(dict, key, buf, MAX);
		ASSERT(written == strlen(value),
		       "Unable to fill buffer with value. Expected %zd writes, got %d writes.\n",
		       strlen(value), written);
		ASSERT(strcmp(buf, value) == 0,
		       "Wrong value read for key %zu. Expected '%s', got '%s'.\n", i, value, buf);
	}

	DictionaryFree(dict);
}

static void TestDeletion()
{
	
	Dictionary *dict = DictionaryInit(0);

	ASSERT(dict != NULL, "Unable to instance a dictionary.\n");

	Error err = DictionarySet(dict, "key1", "value1");
	ASSERT(err == ERR_OK, "Error %s upon setting a dictionary entry.\n",
	       ERROR_TO_STRING(err));


	err = DictionaryErase(dict, "key1");
	ASSERT(err == ERR_OK, "Error %s upon erasing a dictionary entry.\n",
	       ERROR_TO_STRING(err));

	ASSERT(dict->count == 0, "Wrong count set, expected 0, got %zu.\n",
	       dict->count);

	DictionaryFree(dict);
}

static void TestStressDeletion()
{
	
	Dictionary *dict = DictionaryInit(0);
	char key[32];
	char value[32];

	/* Insert a large number of entries */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "%zukey", i);
		snprintf(value, sizeof(value), "%zuvalue", i);
		int err = DictionarySet(dict, key, value);
		ASSERT(err == ERR_OK, "%s upon setting a dictionary entry.\n",
		       ERROR_TO_STRING(err));
		ASSERT(dict->count == i + 1,
		       "expected the count to be of %zu, got %zu\n",
		       i + 1, dict->count);
	}

	/* Delete every other entry */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i += 2) {
		snprintf(key, sizeof(key), "%zukey", i);
		Error err = DictionaryErase(dict, key);
		ASSERT(err == ERR_OK, "%s upon erasing a dictionary entry.\n",
		       ERROR_TO_STRING(err));
		size_t expected = LARGE_DATASET_SIZE - (i + 2) / 2;
		ASSERT(dict->count == expected,
		       "expected the count to be of %zu, got %zu\n",
		       expected, dict->count);
	}

	ASSERT(dict->count == LARGE_DATASET_SIZE / 2,
	       "Wrong count after stress deletion, expected %d, got %zu.\n",
	       LARGE_DATASET_SIZE / 2, dict->count);

	/* Verify remaining entries */
	for (size_t i = 1; i < LARGE_DATASET_SIZE; i += 2) {
		snprintf(key, sizeof(key), "%zukey", i);
		snprintf(value, sizeof(value), "%zuvalue", i);

		char buf[MAX];
		int written = DictionaryGet(dict, key, buf, MAX);
		ASSERT(written == strlen(value),
		       "Unable to fill buffer with value. Expected %zd writes, got %d writes.\n",
		       strlen(value), written);
		ASSERT(strcmp(buf, value) == 0,
		       "Wrong value read for key %zu. Expected '%s', got '%s'.\n",
		       i, value, buf);
	}

	DictionaryFree(dict);
}

/* Compare strings numerically since all generated strings begin with
 * numbers. */
static int CompareStrings(const void *a, const void *b)
{
	int i = atoi(*(char **)a);
	int j = atoi(*(char **)b);
	return i - j;
}

static void TestGettingAllKeys()
{
	Dictionary *dict = DictionaryInit(0);
	char key[32];
	char value[32];

	/* Fill dictionary with input. */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "%zukey", i);
		snprintf(value, sizeof(value), "%zuvalue", i);

		Error err = DictionarySet(dict, key, value);
		ASSERT(err == ERR_OK, "Error %s upon setting a large dataset entry %zu.\n",
			ERROR_TO_STRING(err), i);
	}

	ASSERT(dict->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %zu.\n",
	       LARGE_DATASET_SIZE, dict->count);

	/* Get all keys. */
	size_t capacity = dict->count * sizeof(char*);
	char **keys = malloc(capacity);
	/* seg fault. */
	Error err = DictionaryGetKeys(dict, keys, capacity);
	ASSERT(err == ERR_OK, "%s: unable to get all keys in test\n",
	       ERROR_TO_STRING(err));

	/* Sort to easily compare input with output. */
	qsort(keys, dict->count, sizeof(char*), CompareStrings);

	for (size_t i = 0; i < dict->count; i++) {
		snprintf(key, sizeof(key), "%zukey", i);

		ASSERT(strcmp(key, keys[i]) == 0,
		       "Getting all keys: expected %s, got %s.\n",
		       key, keys[i]);
	}

	free(keys);
	DictionaryFree(dict);
}

static void TestGettingAllValues()
{
	Dictionary *dict = DictionaryInit(0);
	char key[32];
	char value[32];

	/* Fill dictionary with input. */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "%zukey", i);
		snprintf(value, sizeof(value), "%zuvalue", i);

		Error err = DictionarySet(dict, key, value);
		ASSERT(err == ERR_OK, "Error %s upon setting a large dataset entry %zu.\n",
			ERROR_TO_STRING(err), i);
	}

	ASSERT(dict->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %zu.\n",
	       LARGE_DATASET_SIZE, dict->count);

	/* Get all keys. */
	size_t capacity = dict->count * sizeof(char*);
	char **values = malloc(capacity);
	Error err = DictionaryGetValues(dict, values, capacity);
	ASSERT(err == ERR_OK, "%s: unable to get all keys in test\n",
	       ERROR_TO_STRING(err));

	/* Sort to easily compare input with output. */
	qsort(values, dict->count, sizeof(char*), CompareStrings);

	for (size_t i = 0; i < dict->count; i++) {
		snprintf(value, sizeof(value), "%zuvalue", i);

		ASSERT(strcmp(value, values[i]) == 0,
		       "Getting all keys: expected %s, got %s.\n",
		       value, values[i]);
	}

	free(values);
	DictionaryFree(dict);
}

static void TestDuplication()
{
	Dictionary *dict = DictionaryInit(0);
	char key[32];
	char value[32];

	/* Fill dictionary with input. */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "%zukey", i);
		snprintf(value, sizeof(value), "%zuvalue", i);

		Error err = DictionarySet(dict, key, value);
		ASSERT(err == ERR_OK,
		       "Error %s upon setting a large dataset entry %zu.\n",
			ERROR_TO_STRING(err), i);
	}

	ASSERT(dict->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %zu.\n",
	       LARGE_DATASET_SIZE, dict->count);

	Dictionary *dictDup = DictionaryDuplicate(dict);

	ASSERT(dictDup != NULL, "Unable to duplicate dictionary\n");

	ASSERT(dictDup->count == dict->count,
	       "Unexpected entry count in dictionary. Expected %zu, got %zu\n",
	       dict->count, dictDup->count);
	ASSERT(dictDup->capacity == dict->capacity,
	       "Unexpected entry capacity in dictionary. Expected %zu, got %zu\n",
	       dict->capacity, dictDup->capacity);
	ASSERT(dictDup->seed == dict->seed,
	       "Unexpected seed in dictionary. Expected %zu, got %zu\n",
	       dict->seed, dictDup->seed);

	char expectedValue[32];
	char receivedValue[32];
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "%zukey", i);

		int err1 = DictionaryGet(dict, key, expectedValue, 32);
		int err2 = DictionaryGet(dictDup, key, receivedValue, 32);

		ASSERT(err1 != -1,
		       "Unable to retrieve value from original dictionary\n");
		ASSERT(err2 != -1,
		       "Unable to retrieve value from copied of dictionary\n");

		ASSERT(strcmp(expectedValue, receivedValue) == 0,
		       "Duplicated dictionary's data is not the same as the original.\n");
	}

	DictionaryFree(dict);
	DictionaryFree(dictDup);
}

static void TestComparisonsTrue()
{
	Dictionary *dict1 = DictionaryInit(0);
	Dictionary *dict2 = DictionaryInit(0);
	char key[32];
	char value[32];

	/* Fill dictionaries with input. */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "%zukey", i);
		snprintf(value, sizeof(value), "%zuvalue", i);

		Error err = DictionarySet(dict1, key, value);
		ASSERT(err == ERR_OK, "Error %s upon setting a large dataset entry %zu.\n",
			ERROR_TO_STRING(err), i);
		err = DictionarySet(dict2, key, value);
		ASSERT(err == ERR_OK, "Error %s upon setting a large dataset entry %zu.\n",
			ERROR_TO_STRING(err), i);
	}

	ASSERT(dict1->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %zu.\n",
	       LARGE_DATASET_SIZE, dict1->count);
	ASSERT(dict2->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %zu.\n",
	       LARGE_DATASET_SIZE, dict2->count);

	ASSERT(DictionaryCompare(dict1, dict2),
	       "Expected dictionaries to be equal, received unequal\n");

	DictionaryFree(dict1);
	DictionaryFree(dict2);
}

/* Test ruining equality by modifying values. */
static void TestComparisonsFalse1()
{
	Dictionary *dict1 = DictionaryInit(0);
	Dictionary *dict2 = DictionaryInit(0);
	char key[32];
	char value[32];

	/* Fill dictionaries with input. */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "%zukey", i);
		snprintf(value, sizeof(value), "%zuvalue", i);

		Error err = DictionarySet(dict1, key, value);
		ASSERT(err == ERR_OK, "Error %s upon setting a large dataset entry %zu.\n",
			ERROR_TO_STRING(err), i);
		err = DictionarySet(dict2, key, value);
		ASSERT(err == ERR_OK, "Error %s upon setting a large dataset entry %zu.\n",
			ERROR_TO_STRING(err), i);
	}

	/* Ruin the equality. */
	DictionarySet(dict1, "0key", "0value0");

	ASSERT(dict1->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %zu.\n",
	       LARGE_DATASET_SIZE, dict1->count);
	ASSERT(dict2->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %zu.\n",
	       LARGE_DATASET_SIZE, dict2->count);

	ASSERT(!DictionaryCompare(dict1, dict2),
	       "Expected dictionaries to be unequal, received equal\n");

	DictionaryFree(dict1);
	DictionaryFree(dict2);
}

/* Test ruining equality by erasing keys. */
static void TestComparisonsFalse2()
{
	Dictionary *dict1 = DictionaryInit(0);
	Dictionary *dict2 = DictionaryInit(0);
	char key[32];
	char value[32];

	/* Fill dictionaries with input. */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "%zukey", i);
		snprintf(value, sizeof(value), "%zuvalue", i);

		Error err = DictionarySet(dict1, key, value);
		ASSERT(err == ERR_OK, "Error %s upon setting a large dataset entry %zu.\n",
			ERROR_TO_STRING(err), i);
		err = DictionarySet(dict2, key, value);
		ASSERT(err == ERR_OK, "Error %s upon setting a large dataset entry %zu.\n",
			ERROR_TO_STRING(err), i);
	}

	ASSERT(dict1->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %zu.\n",
	       LARGE_DATASET_SIZE, dict1->count);
	ASSERT(dict2->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %zu.\n",
	       LARGE_DATASET_SIZE, dict2->count);

	/* Ruin the equality. */
	DictionaryErase(dict1, "0key");

	ASSERT(!DictionaryCompare(dict1, dict2),
	       "Expected dictionaries to be unequal, received equal\n");

	DictionaryFree(dict1);
	DictionaryFree(dict2);
}

/* Test ruining equality by adding keys. */
static void TestComparisonsFalse3()
{
	Dictionary *dict1 = DictionaryInit(0);
	Dictionary *dict2 = DictionaryInit(0);
	char key[32];
	char value[32];

	/* Fill dictionaries with input. */
	for (size_t i = 0; i < LARGE_DATASET_SIZE; i++) {
		snprintf(key, sizeof(key), "%zukey", i);
		snprintf(value, sizeof(value), "%zuvalue", i);

		Error err = DictionarySet(dict1, key, value);
		ASSERT(err == ERR_OK, "Error %s upon setting a large dataset entry %zu.\n",
			ERROR_TO_STRING(err), i);
		err = DictionarySet(dict2, key, value);
		ASSERT(err == ERR_OK, "Error %s upon setting a large dataset entry %zu.\n",
			ERROR_TO_STRING(err), i);
	}

	ASSERT(dict1->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %zu.\n",
	       LARGE_DATASET_SIZE, dict1->count);
	ASSERT(dict2->count == LARGE_DATASET_SIZE,
	       "Wrong count set for large dataset, expected %d, got %zu.\n",
	       LARGE_DATASET_SIZE, dict2->count);

	/* Ruin the equality. */
	DictionarySet(dict1, "newKey", "newValue");

	ASSERT(!DictionaryCompare(dict1, dict2),
	       "Expected dictionaries to be unequal, received equal\n");

	DictionaryFree(dict1);
	DictionaryFree(dict2);
}

int main(void)
{
	TestInitialization();
	TestInsertion();
	TestLargeDataset();
	TestDeletion();
	TestStressDeletion();
	TestDuplication();
	TestGettingAllKeys();
	TestGettingAllValues();
	TestComparisonsTrue();
	TestComparisonsFalse1();
	TestComparisonsFalse2();
	TestComparisonsFalse3();

	return succeed;
}
