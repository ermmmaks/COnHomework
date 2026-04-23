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

void graphFree(Graph* g)
{
    if (g == NULL) {
        printf("Graph doesn't exist!\n");
        return;
    }
    free(g->head);
    free(g->next);
    free(g->edges);
    free(g);
}

Graph* graphCreate(int citiesCount, int roadsCount)
{
    Graph* g = malloc(sizeof(Graph));
    if (g == NULL) {
        printf("Allocate error!\n");
        return NULL;
    }

    g->citiesCount = citiesCount;
    g->roadsCount = roadsCount;
    g->head = malloc((citiesCount + 1) * sizeof(int));
    g->next = malloc(2 * roadsCount * sizeof(int));
    g->edges = malloc(2 * roadsCount * sizeof(struct Edge));

    if (g->head == NULL || g->next == NULL || g->edges == NULL) {
        graphFree(g);
        printf("Allocate error!\n");
        return NULL;
    }

    for (int i = 1; i <= citiesCount; i++) {
        g->head[i] = -1;
    }
    return g;
}

void graphAdd(Graph* g, int u, int v, int len, int idx)
{
    if (g == NULL || idx >= (2 * g->roadsCount)) {
        printf("Invalid graph or index!\n");
        return;
    }
    g->edges[idx].way = v;
    g->edges[idx].len = len;
    g->next[idx] = g->head[u];
    g->head[u] = idx;
}

void freeAnnexTask(Graph* g, State* s, int capitalsCount, int* citiesOwners)
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

    graphFree(g);
}

// iteration on neighbours

int findNearest(Graph* g, State* s, const int* citiesOwners)
{
    int bestCity = -1;
    int minLen = INT_MAX;

    for (int i = 0; i < s->count; i++) {
        int u = s->cities[i];
        int e = g->head[u];

        while (e != -1) {
            int v = g->edges[e].way;
            int currLen = g->edges[e].len;

            if (citiesOwners[v] == 0) {
                if (bestCity == -1 || currLen < minLen) {
                    minLen = currLen;
                    bestCity = v;
                }
            }
            e = g->next[e];
        }
    }
    return bestCity;
}

/* ordered the cities
for every states one by one search a nearest free city
addedAny -- flag to stop on disconnected graph */

void annex(Graph* g, State* s, int capitalsCount, int* citiesOwners)
{
    int total = capitalsCount;
    while (total < g->citiesCount) {
        bool addedAny = false;
        for (int i = 0; i < capitalsCount; i++) {
            if (total >= g->citiesCount) {
                break;
            }
            int nextVer = findNearest(g, &s[i], citiesOwners);
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
