#pragma once

#include <stdbool.h>

typedef struct Edge Edge;

typedef struct Graph Graph;

typedef struct State State;

// create new states structure. return NULL on memory allocation failure
State* statesCreate(int count);

/* init state, allocate memory for cities list,
set the states Id, add the capital as city */
void stateInit(State* state, int idx, int stateId, int capital, int maxCities);

// return Id of the state
int stateGetId(State* state, int idx);

// return count of the cities of the states
int stateGetCount(State* state, int idx);

// return city by state and city index
int stateGetCity(State* state, int stateIdx, int cityIdx);

// create new graph. return NULL on memory allocation failure
Graph* graphCreate(int citiesCount, int roadsCount);

// add edge to list of neighbours
void graphAdd(Graph* graph, int u, int v, int len, int idx);

// free function for all allocated memory
void freeAnnexTask(Graph* graph, State* s, int capitalsCount, int* citiesOwners);

// find nearest free sity for accepted state
int findNearest(Graph* graph, State* s, const int* citiesOwners);

// function for annexation the cities
void annex(Graph* graph, State* s, int capitalsCount, int* citiesOwners);
