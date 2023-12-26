//map.h 
#ifndef MAP_H
#define MAP_H

typedef struct {
    struct KeyValuePair *data;
    int size;
    int capacity;
} Map;

void initializeMap(Map *map, int initialCapacity);
void insertIntoMap(Map *map, const char *key, int value);
int getFromMap(Map *map, const char *key);
void freeMap(Map *map);

#endif // !MAP_H
