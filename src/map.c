#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct KeyValuePair {
  char *key;
  int value;
};

void initializeMap(Map *map, int initialCapacity) {
  map->data = malloc(initialCapacity * sizeof(struct KeyValuePair));
  map->size = 0;
  map->capacity = initialCapacity;
}

void insertIntoMap(Map *map, const char *key, int value) {
  // Check if the key already exists
  for (int i = 0; i < map->size; i++) {
    if (strcmp(map->data[i].key, key) == 0) {
      // Key already exists, update the value
      map->data[i].value = value;
      return;
    }
  }

  // Key does not exist, insert a new key-value pair
  if (map->size == map->capacity) {
    // Double the capacity if the map is full
    map->capacity *= 2;
    map->data = realloc(map->data, map->capacity * sizeof(struct KeyValuePair));
  }

  // Allocate memory for the key and copy it
  map->data[map->size].key = strdup(key);
  map->data[map->size].value = value;
  map->size++;
}

int getFromMap(Map *map, const char *key) {
  for (int i = 0; i < map->size; i++) {
    if (strcmp(map->data[i].key, key) == 0) {
      return map->data[i].value;
    }
  }

  // Return a default value (you may choose to handle this differently)
  return -1;
}

void freeMap(Map *map) {
  for (int i = 0; i < map->size; i++) {
    free(map->data[i].key);
  }
  free(map->data);
}
