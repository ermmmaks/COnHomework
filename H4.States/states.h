#pragma once

#include <stdbool.h>

typedef struct Edge Edge;

typedef struct Graph Graph;

typedef struct State State;

Graph* graphCreate(int n, int m);
void graphAdd(Graph* g, int u, int v, int len, int idx);
void graphFree(Graph* g);
void cleaning(Graph* g, State* s, int k, int* sityesOwners);

int findNearest(Graph* g, State* s, const int* sityesOwners);
void annex(Graph* g, State* s, int k, int* sityesOwners);
