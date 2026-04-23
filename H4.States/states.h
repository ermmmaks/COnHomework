#pragma once

#include <stdbool.h>

typedef struct Edge Edge;

typedef struct Graph Graph;

typedef struct State State;

// create new graph. return NULL on memory allocation failure

Graph* graphCreate(int citiesCount, int roadsCount);

// add edge to list of neighbours

void graphAdd(Graph* g, int u, int v, int len, int idx);

// free function for all allocated memory

void freeAnnexTask(Graph* g, State* s, int capitalsCount, int* citiesOwners);

// find nearest free sity for accepted state

int findNearest(Graph* g, State* s, const int* citiesOwners);

// function for annexation the cities

void annex(Graph* g, State* s, int capitalsCount, int* citiesOwners);
