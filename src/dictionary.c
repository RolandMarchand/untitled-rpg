#include "dictionary.h"

#define DICTIONARY_MAX_LOAD 0.75
enum {
	DICTIONARY_MAGNITUDE_INCREASE = 2,
};

typedef enum {
	ENTRY_STATE_EMPTY = 0,
	ENTRY_STATE_FILLED,
	ENTRY_STATE_ZOMBIE
} EntryState;

struct Entry {
	char *key;
	char *value;
	uint64_t hash;
	EntryState state;
};

static Error DictionaryIncreaseSize(Dictionary *dict);
static uint64_t Murmur3Hash(const void *key, size_t len, uint64_t seed);

Dictionary *DictionaryInit(size_t capacity)
{
	Dictionary *map = malloc(sizeof(Dictionary));
	if (map == NULL) {
		perror("Failed to allocate dictionary.");
		return NULL;
	}

	if (capacity == 0) {
		capacity = DICTIONARY_DEFAULT_CAPACITY;
	}

	map->entries = calloc(capacity, sizeof(Entry));
	if (map->entries == NULL) {
		perror("Failed to allocate dictionary.");
		free(map);
		return NULL;
	}

	map->count = 0;
	map->capacity = capacity;
	map->seed = (uint64_t)rand();
	return map;
}

void DictionaryFree(Dictionary *dict)
{
	if (dict == NULL) {
		return;
	}

	for (size_t i = 0; i < dict->capacity; i++) {
		if (dict->entries[i].state == ENTRY_STATE_FILLED) {
			free(dict->entries[i].key);
			free(dict->entries[i].value);
		}
	}

	free(dict->entries);
	free(dict);
}

Dictionary *DictionaryDuplicate(const Dictionary *dict)
{
	Dictionary *ret = malloc(sizeof(Dictionary));
	if (ret == NULL) {
		return NULL;
	}

	ret->count = dict->count;
	ret->capacity = dict->capacity;
	ret->seed = dict->seed;
	ret->entries = calloc(ret->capacity, sizeof(Entry));
	if (ret->entries == NULL) {
		free(ret);
		return NULL;
	}

	for (size_t i = 0; i < ret->capacity; i++) {
		ret->entries[i].hash = dict->entries[i].hash;
		ret->entries[i].state = dict->entries[i].state;

		if (ret->entries[i].state != ENTRY_STATE_FILLED) {
			continue;
		}

		ret->entries[i].key = strdup(dict->entries[i].key);
		ret->entries[i].value = strdup(dict->entries[i].value);

		if (ret->entries[i].key == NULL ||
		    ret->entries[i].value == NULL) {
			DictionaryFree(ret);
			return NULL;
		}
	}
	return ret;
}

int DictionaryGet(Dictionary *dict, const char *key, char *out, size_t capacity)
{
	if (dict == NULL || key == NULL || out == NULL) {
		return -1;
	}

	uint64_t hash = Murmur3Hash(key, strlen(key), dict->seed);
	uint64_t idx = hash % dict->capacity;
	for (size_t i = 0; i < dict->capacity; i++) {
		switch (dict->entries[idx].state) {
		case ENTRY_STATE_EMPTY:
			if (capacity > 0) {
				out[0] = '\0';
			}
			return -1;

		case ENTRY_STATE_FILLED:
			if (dict->entries[idx].hash == hash &&
			    strcmp(dict->entries[idx].key, key) == 0) {
				const char *value = dict->entries[idx].value;
				return snprintf(out, capacity, "%s", value);
			}

			/* fall-through */
		case ENTRY_STATE_ZOMBIE:
			idx = (idx + 1) % dict->capacity;
		}
	}

	/* The dictionary is full because it cannot expand further. Unlikely. */
	return -1;
}

Error DictionarySet(Dictionary *dict, const char *key, const char *value)
{
	if (dict == NULL || key == NULL || value == NULL) {
		return ERR_INVALID_ARGUMENT;
	}

	uint64_t hash = Murmur3Hash(key, strlen(key), dict->seed);
	uint64_t idx = hash % dict->capacity;
	for (size_t i = 0; i < dict->capacity; i++) {
		switch (dict->entries[idx].state) {
		case ENTRY_STATE_ZOMBIE:
		case ENTRY_STATE_EMPTY:
			dict->entries[idx].key = strdup(key);
			dict->entries[idx].value = strdup(value);
			dict->entries[idx].hash = hash;
			dict->entries[idx].state = ENTRY_STATE_FILLED;
			dict->count++;
			if (dict->count >
			    dict->capacity * DICTIONARY_MAX_LOAD) {
				Error err = DictionaryIncreaseSize(dict);
				if (err != ERR_OK) {
					return err;
				}
			}
			return ERR_OK;

		case ENTRY_STATE_FILLED:
			if (dict->entries[idx].hash == hash &&
			    strcmp(dict->entries[idx].key, key) == 0) {
				free(dict->entries[idx].value);
				dict->entries[idx].value = strdup(value);
				return ERR_OK;
			}
		}
		idx = (idx + 1) % dict->capacity;
	}

	/* The dictionary is full because it cannot expand further. Unlikely. */
	return ERR_OUT_OF_MEMORY;
}

Error DictionaryErase(Dictionary *dict, const char *key)
{
	if (dict == NULL || key == NULL) {
		return ERR_INVALID_ARGUMENT;
	}

	uint64_t hash = Murmur3Hash(key, strlen(key), dict->seed);
	uint64_t idx = hash % dict->capacity;
	for (size_t i = 0; i < dict->capacity; i++) {
		switch (dict->entries[idx].state) {
		case ENTRY_STATE_EMPTY:
			return ERR_OK;

		case ENTRY_STATE_FILLED:
			if (dict->entries[idx].hash == hash &&
			    strcmp(dict->entries[idx].key, key) == 0) {
				free(dict->entries[idx].key);
				free(dict->entries[idx].value);
				dict->entries[idx].state = ENTRY_STATE_ZOMBIE;
				dict->count--;
				return ERR_OK;
			}
			/* fall-through */

		case ENTRY_STATE_ZOMBIE:
			idx = (idx + 1) % dict->capacity;
		}
	}

	/* The dictionary is full because it cannot expand further. Unlikely. */
	return ERR_OUT_OF_MEMORY;
}

Error DictionaryGetKeys(Dictionary *dict, char **out, size_t capacity)
{
	if (dict == NULL || out == NULL) {
		return ERR_NULL_REFERENCE;
	}

	if (capacity < dict->count * sizeof(char *)) {
		return ERR_INSUFFICIENT_SPACE;
	}

	int outIdx = 0;
	for (size_t dictIdx = 0; dictIdx < dict->capacity; dictIdx++) {
		if (dict->entries[dictIdx].state != ENTRY_STATE_FILLED) {
			continue;
		}
		/* seg fault */
		out[outIdx] = dict->entries[dictIdx].key;
		outIdx++;
	}

	return ERR_OK;
}

Error DictionaryGetValues(Dictionary *dict, char **out, size_t capacity)
{
	if (dict == NULL || out == NULL) {
		return ERR_NULL_REFERENCE;
	}

	if (capacity < dict->count * sizeof(char *)) {
		return ERR_INSUFFICIENT_SPACE;
	}

	int outIdx = 0;
	for (size_t dictIdx = 0; dictIdx < dict->capacity; dictIdx++) {
		if (dict->entries[dictIdx].state != ENTRY_STATE_FILLED) {
			continue;
		}
		/* seg fault */
		out[outIdx] = dict->entries[dictIdx].value;
		outIdx++;
	}

	return ERR_OK;
}

static int DictionaryCompareInternal(const void *a, const void *b)
{
	const char *aStr = *(const char **)a;
	const char *bStr = *(const char **)b;
	return strcmp(aStr, bStr);
}

bool DictionaryCompare(Dictionary *dict1, Dictionary *dict2)
{
	/* Compare pointers. */
	if (dict1 == NULL || dict2 == NULL) {
		return false;
	}
	if (dict1 == dict2) {
		return true;
	}
	if (dict1->count != dict2->count) {
		return false;
	}
	if (dict1->entries == dict2->entries) {
		return true;
	}

	size_t allocSpace = dict1->count * sizeof(char *);

	/* Compare keys. */
	/* Allocate space for keys. */
	char **dict1Keys = malloc(allocSpace);
	if (dict1Keys == NULL) {
		return ERR_OUT_OF_MEMORY;
	}
	char **dict2Keys = malloc(allocSpace);
	if (dict2Keys == NULL) {
		free(dict1Keys);
		return ERR_OUT_OF_MEMORY;
	}

	/* Fetch keys. */
	Error err = DictionaryGetKeys(dict1, dict1Keys, allocSpace);
	if (err != ERR_OK) {
		free(dict1Keys);
		free(dict2Keys);
		return false;
	}
	err = DictionaryGetKeys(dict2, dict2Keys, allocSpace);
	if (err != ERR_OK) {
		free(dict1Keys);
		free(dict2Keys);
		return false;
	}

	/* Sort keys for easy comparison. */
	qsort(dict1Keys, dict1->count, sizeof(char *),
	      DictionaryCompareInternal);
	qsort(dict2Keys, dict2->count, sizeof(char *),
	      DictionaryCompareInternal);

	/* Compare sorted keys. */
	for (size_t i = 0; i < dict1->count; i++) {
		if (strcmp(dict1Keys[i], dict2Keys[i]) != 0) {
			free(dict1Keys);
			free(dict2Keys);
			return false;
		}
	}
	free(dict1Keys);
	free(dict2Keys);

	/* Compare values. */
	/* Allocate space for values. */
	char **dict1Values = malloc(allocSpace);
	if (dict1Values == NULL) {
		return ERR_OUT_OF_MEMORY;
	}
	char **dict2Values = malloc(allocSpace);
	if (dict2Values == NULL) {
		free(dict1Values);
		return ERR_OUT_OF_MEMORY;
	}

	/* Fetch values. */
	err = DictionaryGetValues(dict1, dict1Values, allocSpace);
	if (err != ERR_OK) {
		free(dict1Values);
		free(dict2Values);
		return false;
	}
	err = DictionaryGetValues(dict2, dict2Values, allocSpace);
	if (err != ERR_OK) {
		free(dict1Values);
		free(dict2Values);
		return false;
	}

	/* Sort values for easy comparison. */
	qsort(dict1Values, dict1->count, sizeof(char *),
	      DictionaryCompareInternal);
	qsort(dict2Values, dict2->count, sizeof(char *),
	      DictionaryCompareInternal);

	/* Compare sorted values. */
	for (size_t i = 0; i < dict1->count; i++) {
		if (strcmp(dict1Values[i], dict2Values[i]) != 0) {
			free(dict1Values);
			free(dict2Values);
			return false;
		}
	}
	free(dict1Values);
	free(dict2Values);

	return true;
}

Error DictionaryIncreaseSize(Dictionary *dict)
{
	Dictionary *bigDict =
		DictionaryInit(dict->capacity * DICTIONARY_MAGNITUDE_INCREASE);
	if (bigDict == NULL) {
		return ERR_OUT_OF_MEMORY;
	}

	bigDict->seed = dict->seed;

	for (size_t i = 0; i < dict->capacity; i++) {
		if (dict->entries[i].state != ENTRY_STATE_FILLED) {
			continue;
		}

		uint64_t bigIdx = dict->entries[i].hash % bigDict->capacity;
		size_t bigIter = 0;
		while (bigDict->entries[bigIdx].state == ENTRY_STATE_FILLED &&
		       likely(bigIter < bigDict->capacity)) {
			bigIdx = (bigIdx + 1) % bigDict->capacity;
			bigIter++;
		}

		if (unlikely(bigIter >= bigDict->capacity)) {
			perror("Error while increasing size of dictionary.");
			return ERR_UNKNOWN;
		}

		bigDict->entries[bigIdx].key = dict->entries[i].key;
		bigDict->entries[bigIdx].value = dict->entries[i].value;
		bigDict->entries[bigIdx].hash = dict->entries[i].hash;
		bigDict->entries[bigIdx].state = ENTRY_STATE_FILLED;
		bigDict->count++;
	}

	free(dict->entries);
	*dict = *bigDict;
	free(bigDict);

	return ERR_OK;
}

static uint64_t Rotl(uint64_t x, int8_t r)
{
	return (x << r) | (x >> (64 - r));
}

static uint64_t Fmix(uint64_t k)
{
	k ^= k >> 33;
	k *= 0xff51afd7ed558ccdLLU;
	k ^= k >> 33;
	k *= 0xc4ceb9fe1a85ec53LLU;
	k ^= k >> 33;

	return k;
}

uint64_t Murmur3Hash(const void *key, size_t len, uint64_t seed)
{
	const uint8_t *data = (const uint8_t *)key;
	const size_t nblocks = len / 16;
	size_t i;

	uint64_t h1 = seed;
	uint64_t h2 = seed;

	uint64_t c1 = 0x87c37b91114253d5LLU;
	uint64_t c2 = 0x4cf5ad432745937fLLU;

	const uint64_t *blocks = (const uint64_t *)(data);

	for (i = 0; i < nblocks; i++) {
		uint64_t k1 = blocks[i * 2 + 0];
		uint64_t k2 = blocks[i * 2 + 1];

		k1 *= c1;
		k1 = Rotl(k1, 31);
		k1 *= c2;
		h1 ^= k1;
		h1 = Rotl(h1, 27);
		h1 += h2;
		h1 = h1 * 5 + 0x52dce729;
		k2 *= c2;
		k2 = Rotl(k2, 33);
		k2 *= c1;
		h2 ^= k2;
		h2 = Rotl(h2, 31);
		h2 += h1;
		h2 = h2 * 5 + 0x38495ab5;
	}

	const uint8_t *tail = (const uint8_t *)(data + nblocks * 16);

	uint64_t k1 = 0;
	uint64_t k2 = 0;

	switch (len & 15) {
	case 15:
		k2 ^= (uint64_t)(tail[14]) << 48;
		/* fall-through */
	case 14:
		k2 ^= (uint64_t)(tail[13]) << 40;
		/* fall-through */
	case 13:
		k2 ^= (uint64_t)(tail[12]) << 32;
		/* fall-through */
	case 12:
		k2 ^= (uint64_t)(tail[11]) << 24;
		/* fall-through */
	case 11:
		k2 ^= (uint64_t)(tail[10]) << 16;
		/* fall-through */
	case 10:
		k2 ^= (uint64_t)(tail[9]) << 8;
		/* fall-through */
	case 9:
		k2 ^= (uint64_t)(tail[8]) << 0;
		k2 *= c2;
		k2 = Rotl(k2, 33);
		k2 *= c1;
		h2 ^= k2;
		/* fall-through */
	case 8:
		k1 ^= (uint64_t)(tail[7]) << 56;
		/* fall-through */
	case 7:
		k1 ^= (uint64_t)(tail[6]) << 48;
		/* fall-through */
	case 6:
		k1 ^= (uint64_t)(tail[5]) << 40;
		/* fall-through */
	case 5:
		k1 ^= (uint64_t)(tail[4]) << 32;
		/* fall-through */
	case 4:
		k1 ^= (uint64_t)(tail[3]) << 24;
		/* fall-through */
	case 3:
		k1 ^= (uint64_t)(tail[2]) << 16;
		/* fall-through */
	case 2:
		k1 ^= (uint64_t)(tail[1]) << 8;
		/* fall-through */
	case 1:
		k1 ^= (uint64_t)(tail[0]) << 0;
		k1 *= c1;
		k1 = Rotl(k1, 31);
		k1 *= c2;
		h1 ^= k1;
	};

	h1 ^= len;
	h2 ^= len;

	h1 += h2;
	h2 += h1;

	h1 = Fmix(h1);
	h2 = Fmix(h2);

	h1 += h2;
	/* h2 += h1; */

	/* 'h2' is also a high entropy value. */
	return h1;
}
