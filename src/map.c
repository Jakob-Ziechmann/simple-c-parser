#include "map.h"
#include "tree.h"
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

void recursiveInserting(Map* map, Ast* tree);

// collects all identifyer and inserst them with val 0 in map
Map* generateMapFromTree(Ast* tree) {
  // create a reasonable sized map for an expression
  Map* map = (Map*)malloc(sizeof(Map));
  initializeMap(map, 8);

  recursiveInserting(map, tree);

  return map;
}

void recursiveInserting(Map* map, Ast* tree) {
  switch (tree->tag) {
    case Ast_identifier: {
      struct Ast_identifier data = tree->data.Ast_identifier;
      insertIntoMap(map, data.identifier, 0);
    }
    break;

    case Ast_and: {
      struct Ast_and data = tree->data.Ast_and;
      recursiveInserting(map, data.left);
      recursiveInserting(map, data.right);
    }
    break;

    case Ast_eq: {
      struct Ast_eq data = tree->data.Ast_eq;
      recursiveInserting(map, data.left);
      recursiveInserting(map, data.right);
    }
    break;

    case Ast_li: {
      struct Ast_li data = tree->data.Ast_li;
      recursiveInserting(map, data.left);
      recursiveInserting(map, data.right);
    }
    break;


    case Ast_not: {
      struct Ast_not data = tree->data.Ast_not;
      recursiveInserting(map, data.right);
    }
    break;

    case Ast_or: {
      struct Ast_or data = tree->data.Ast_or;
      recursiveInserting(map, data.left);
      recursiveInserting(map, data.right);
    }
    break;

    case Ast_ri: {
      struct Ast_ri data = tree->data.Ast_ri;
      recursiveInserting(map, data.left);
      recursiveInserting(map, data.right);
    }
    break;

    case Ast_literal_false:
    case Ast_literal_true:
    case Ast_empty:
    break;
  }
}
