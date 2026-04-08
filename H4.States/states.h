#pragma once

#include <stdbool.h>

typedef struct Edge Edge;

typedef struct Graph Graph;

typedef struct State State;

// create new graph. return NULL on memory allocation failure

Graph* graphCreate(int n, int m);

// add edge to list of neighbours

void graphAdd(Graph* g, int u, int v, int len, int idx);

// total free-f for all allocated memory

void cleaning(Graph* g, State* s, int k, int* sitiesOwners);

// find nearest free sity for accepted state

int findNearest(Graph* g, State* s, const int* sitiesOwners);

// f for annexation the sities

void annex(Graph* g, State* s, int k, int* sitiesOwners);
