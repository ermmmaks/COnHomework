#include "states.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Edge {
    int way;
    int len;
};

struct Graph {
    int citiesCount;
    int roadsCount;
    int* head;
    int* next;
    Edge* edges;
};

struct State {
    int id;
    int* cities;
    int count;
};

void graphFree(Graph* graph)
{
    if (graph == NULL) {
        printf("Graph doesn't exist!\n");
        return;
    }
    free(graph->head);
    free(graph->next);
    free(graph->edges);
    free(graph);
}

State* statesCreate(int count)
{
    State* states = malloc(count * sizeof(State));
    if (states == NULL) {
        printf("Allocate error!\n");
        return NULL;
    }
    return states;
}

void stateInit(State* state, int idx, int stateId, int capital, int maxCities)
{
    state[idx].id = stateId;
    state[idx].count = 0;
    state[idx].cities = malloc(maxCities * sizeof(int));
    state[idx].cities[state[idx].count++] = capital;
}

int stateGetId(State* state, int idx)
{
    return state[idx].id;
}

int stateGetCount(State* state, int idx)
{
    return state[idx].count;
}

int stateGetCity(State* state, int stateIdx, int cityIdx)
{
    return state[stateIdx].cities[cityIdx];
}

Graph* graphCreate(int citiesCount, int roadsCount)
{
    Graph* graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        printf("Allocate error!\n");
        return NULL;
    }

    graph->citiesCount = citiesCount;
    graph->roadsCount = roadsCount;
    graph->head = malloc((citiesCount + 1) * sizeof(int));
    graph->next = malloc(2 * roadsCount * sizeof(int));
    graph->edges = malloc(2 * roadsCount * sizeof(struct Edge));

    if (graph->head == NULL || graph->next == NULL || graph->edges == NULL) {
        graphFree(graph);
        printf("Allocate error!\n");
        return NULL;
    }

    for (int i = 1; i <= citiesCount; i++) {
        graph->head[i] = -1;
    }
    return graph;
}

void graphAdd(Graph* graph, int u, int v, int len, int idx)
{
    if (graph == NULL || idx >= (2 * graph->roadsCount)) {
        printf("Invalid graph or index!\n");
        return;
    }
    graph->edges[idx].way = v;
    graph->edges[idx].len = len;
    graph->next[idx] = graph->head[u];
    graph->head[u] = idx;
}

void freeAnnexTask(Graph* graph, State* s, int capitalsCount, int* citiesOwners)
{
    if (s) {
        for (int i = 0; i < capitalsCount; i++) {
            if (s[i].cities) {
                free(s[i].cities);
            }
        }
        free(s);
    } else {
        printf("States isn't found!\n");
    }

    if (citiesOwners) {
        free(citiesOwners);
    } else {
        printf("Cities owners isn't found!\n");
    }

    graphFree(graph);
}

// iteration on neighbours
int findNearest(Graph* graph, State* s, const int* citiesOwners)
{
    int bestCity = -1;
    int minLen = INT_MAX;

    for (int i = 0; i < s->count; i++) {
        int u = s->cities[i];
        int e = graph->head[u];

        while (e != -1) {
            int v = graph->edges[e].way;
            int currLen = graph->edges[e].len;

            if (citiesOwners[v] == 0) {
                if (bestCity == -1 || currLen < minLen) {
                    minLen = currLen;
                    bestCity = v;
                }
            }
            e = graph->next[e];
        }
    }
    return bestCity;
}

/* ordered the cities
for every states one by one search a nearest free city
addedAny -- flag to stop on disconnected graph */
void annex(Graph* graph, State* s, int capitalsCount, int* citiesOwners)
{
    int total = capitalsCount;
    while (total < graph->citiesCount) {
        bool addedAny = false;
        for (int i = 0; i < capitalsCount; i++) {
            if (total >= graph->citiesCount) {
                break;
            }
            int nextVer = findNearest(graph, &s[i], citiesOwners);
            if (nextVer != -1) {
                citiesOwners[nextVer] = s[i].id;
                s[i].cities[s[i].count++] = nextVer;
                total++;
                addedAny = true;
            }
        }
        if (!addedAny) {
            break;
        }
    }
}
