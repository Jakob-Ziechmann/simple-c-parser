//map.h 
#ifndef MAP_H
#define MAP_H

#include "tree.h"

typedef struct {
    struct KeyValuePair *data;
    int size;
    int capacity;
} Map;

void initializeMap(Map *map, int initialCapacity);
void insertIntoMap(Map *map, const char *key, int value);
int getFromMap(Map *map, const char *key);
void freeMap(Map *map);
Map* generateMapFromTree(Ast* tree);

#endif // !MAP_H
