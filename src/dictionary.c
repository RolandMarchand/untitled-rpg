#include "dictionary.h"

#define DICTIONARY_MAX_LOAD 0.75
#define DICTIONARY_MAGNITUDE_INCREASE 2

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
static uint64_t Murmur3Hash(const void *key, size_t len, uint32_t seed);

Dictionary *DictionaryInit(size_t size)
{
	Dictionary *map = malloc(sizeof(Dictionary));
	if (map == NULL) {
		perror("Failed to allocate dictionary.");
		return NULL;
	}

	if (size == 0) {
		size = DICTIONARY_DEFAULT_SIZE;
	}

	map->entries = calloc(size, sizeof(Entry));
	if (map->entries == NULL) {
		perror("Failed to allocate dictionary.");
		free(map);
		return NULL;
	}

	/* Prevent freeing unallocated memory. */
	for (size_t i = 0; i < size; i++) {
		map->entries[i].key = NULL;
		map->entries[i].value = NULL;
		map->entries[i].state = ENTRY_STATE_EMPTY;
	}

	map->count = 0;
	map->size = size;
	map->seed = (uint32_t)rand();
	return map;
}

void DictionaryFree(Dictionary *dict)
{
	if (dict == NULL) {
		return;
	}

	for (size_t i = 0; i < dict->size; i++) {
		if (dict->entries[i].state == ENTRY_STATE_FILLED) {
			void *key = dict->entries[i].key;
			void *value = dict->entries[i].value;
			free(key);
			free(value);
		}
	}

	free(dict->entries);
	free(dict);
}

int DictionaryGet(Dictionary *dict, const char *key, char *out, size_t capacity)
{
	if (dict == NULL || key == NULL || out == NULL) {
		return -1;
	}

	uint64_t hash = Murmur3Hash(key, strlen(key), dict->seed);
	uint64_t idx = hash % dict->size;
	for (size_t i = 0; i < dict->size; i++) {
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
			idx = (idx + 1) % dict->size;
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
	uint64_t idx = hash % dict->size;
	for (size_t i = 0; i < dict->size; i++) {
		switch (dict->entries[idx].state) {
		case ENTRY_STATE_ZOMBIE:
		case ENTRY_STATE_EMPTY:
			dict->entries[idx].key = strdup(key);
			dict->entries[idx].value = strdup(value);
			dict->entries[idx].hash = hash;
			dict->entries[idx].state = ENTRY_STATE_FILLED;
			dict->count++;
			if (dict->count > dict->size * DICTIONARY_MAX_LOAD) {
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
		idx = (idx + 1) % dict->size;
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
	uint64_t idx = hash % dict->size;
	for (size_t i = 0; i < dict->size; i++) {
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
			idx = (idx + 1) % dict->size;
		}
	}

	/* The dictionary is full because it cannot expand further. Unlikely. */
	return ERR_OUT_OF_MEMORY;
}

Error DictionaryIncreaseSize(Dictionary *dict)
{
	Dictionary *bigDict =
		DictionaryInit(dict->size * DICTIONARY_MAGNITUDE_INCREASE);
	if (bigDict == NULL) {
		return ERR_OUT_OF_MEMORY;
	}

	bigDict->seed = dict->seed;

	for (size_t i = 0; i < dict->size; i++) {
		if (dict->entries[i].state != ENTRY_STATE_FILLED) {
			continue;
		}

		uint64_t bigIdx = dict->entries[i].hash % bigDict->size;
		size_t bigIter = 0;
		while (bigDict->entries[bigIdx].state == ENTRY_STATE_FILLED &&
		       likely(bigIter < bigDict->size)) {
			bigIdx = (bigIdx + 1) % bigDict->size;
			bigIter++;
		}

		if (unlikely(bigIter >= bigDict->size)) {
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

static uint64_t rotl(uint64_t x, int8_t r)
{
	return (x << r) | (x >> (64 - r));
}

static uint64_t fmix(uint64_t k)
{
	k ^= k >> 33;
	k *= 0xff51afd7ed558ccdLLU;
	k ^= k >> 33;
	k *= 0xc4ceb9fe1a85ec53LLU;
	k ^= k >> 33;

	return k;
}

uint64_t Murmur3Hash(const void *key, size_t len, uint32_t seed)
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
		k1 = rotl(k1, 31);
		k1 *= c2;
		h1 ^= k1;
		h1 = rotl(h1, 27);
		h1 += h2;
		h1 = h1 * 5 + 0x52dce729;
		k2 *= c2;
		k2 = rotl(k2, 33);
		k2 *= c1;
		h2 ^= k2;
		h2 = rotl(h2, 31);
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
		k2 = rotl(k2, 33);
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
		k1 = rotl(k1, 31);
		k1 *= c2;
		h1 ^= k1;
	};

	h1 ^= len;
	h2 ^= len;

	h1 += h2;
	h2 += h1;

	h1 = fmix(h1);
	h2 = fmix(h2);

	h1 += h2;
	h2 += h1;

	/* 'h2' is also a high entropy value. */
	return h1;
}
