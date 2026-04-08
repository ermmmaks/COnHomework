#include "states.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Edge {
    int way;
    int len;
};

struct Graph {
    int n;
    int m;
    int* head;
    int* next;
    Edge* edges;
};

struct State {
    int id;
    int* sities;
    int count;
};

Graph* graphCreate(int n, int m)
{
    Graph* g = malloc(sizeof(Graph));
    if (g == NULL) {
        printf("Allocate error!\n");
        return NULL;
    }

    g->n = n;
    g->m = m;
    g->head = malloc((n + 1) * sizeof(int));
    g->next = malloc(2 * m * sizeof(int));
    g->edges = malloc(2 * m * sizeof(struct Edge));

    if (g->head == NULL || g->next == NULL || g->edges == NULL) {
        graphFree(g);
        printf("Allocate error!\n");
        return NULL;
    }

    for (int i = 1; i <= n; i++) {
        g->head[i] = -1;
    }
    return g;
}

void graphAdd(Graph* g, int u, int v, int len, int idx)
{
    if (g == NULL || idx >= (2 * g->m)) {
        printf("Invalid graph or index!\n");
        return;
    }
    g->edges[idx].way = v;
    g->edges[idx].len = len;
    g->next[idx] = g->head[u];
    g->head[u] = idx;
}

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

void cleaning(Graph* g, State* s, int k, int* sitiesOwners)
{
    if (s) {
        for (int i = 0; i < k; i++) {
            if (s[i].sities) {
                free(s[i].sities);
            }
        }
        free(s);
    } else {
        printf("States isn't found!\n");
    }

    if (sitiesOwners) {
        free(sitiesOwners);
    } else {
        printf("Sities owners isn't found!\n");
    }

    graphFree(g);
}

int findNearest(Graph* g, State* s, const int* sitiesOwners)
{
    int bestCity = -1;
    int minLen = INT_MAX;

    for (int i = 0; i < s->count; i++) {
        int u = s->sities[i];
        int e = g->head[u];

        while (e != -1) {
            int v = g->edges[e].way;
            int currLen = g->edges[e].len;

            if (sitiesOwners[v] == 0) {
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

void annex(Graph* g, State* s, int k, int* sitiesOwners)
{
    int total = k;
    while (total < g->n) {
        bool addedAny = false;
        for (int i = 0; i < k; i++) {
            if (total >= g->n) {
                break;
            }
            int nextVer = findNearest(g, &s[i], sitiesOwners);
            if (nextVer != -1) {
                sitiesOwners[nextVer] = s[i].id;
                s[i].sities[s[i].count++] = nextVer;
                total++;
                addedAny = true;
            }
        }
        if (!addedAny) {
            break;
        }
    }
}
