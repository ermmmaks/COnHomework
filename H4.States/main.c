#include "states.h"
#include <stdio.h>
#include <stdlib.h>

struct State {
    int id;
    int* sityes;
    int count;
};

void globalFree(Graph* g, State* s, int k, int* sityesOwners)
{
    if (s) {
        for (int i = 0; i < k; i++) {
            if (s[i].sityes) {
                free(s[i].sityes);
            }
        }
        free(s);
    } else {
        printf("States isn't found!\n");
    }

    if (sityesOwners) {
        free(sityesOwners);
    } else {
        printf("Sityes owners isn't found!\n");
    }

    graphFree(g);
}

int main(void)
{
    int n;
    int m;
    if (scanf("%d %d", &n, &m) != 2) {
        return 0;
    }

    Graph* g = graphCreate(n, m);
    if (g == NULL) {
        printf("Graph doesn't exist!\n");
        return 1;
    }

    for (int i = 0; i < m; i++) {
        int u;
        int v;
        int len;
        if (scanf("%d %d %d", &u, &v, &len) != 3) {
            break;
        }
        graphAdd(g, u, v, len, 2 * i);
        graphAdd(g, v, u, len, 2 * i + 1);
    }

    int k;
    if (scanf("%d", &k) != 1) {
        globalFree(g, NULL, 0, NULL);
        return 1;
    }

    State* states = malloc(k * sizeof(State));
    int* sityesOwners = calloc(n + 1, sizeof(int));

    if (states == NULL || sityesOwners == NULL) {
        printf("Allocate error!\n");
        globalFree(g, states, 0, sityesOwners);
        return 1;
    }

    for (int i = 0; i < k; i++) {
        int capital;
        if (scanf("%d", &capital) != 1) {
            break;
        }

        states[i].id = i + 1;
        states[i].count = 0;
        states[i].sityes = malloc(n * sizeof(int));

        if (states[i].sityes == NULL) {
            globalFree(g, states, i, sityesOwners);
            return 1;
        }

        states[i].sityes[states[i].count++] = capital;
        sityesOwners[capital] = i + 1;
    }

    annexation(g, states, k, sityesOwners);

    for (int i = 0; i < k; i++) {
        printf("State #%d: ", states[i].id);
        for (int j = 0; j < states[i].count; j++) {
            printf("%d ", states[i].sityes[j]);
        }
        printf("\n");
    }

    globalFree(g, states, k, sityesOwners);

    return 0;
}
