#include "states.h"
#include <stdio.h>
#include <stdlib.h>

struct State {
    int id;
    int* sities;
    int count;
};

int main(void)
{
    int n;
    int m;
    printf("Write the count of sities and ways: ");
    if (scanf("%d %d", &n, &m) != 2) {
        return 0;
    }

    Graph* g = graphCreate(n, m);
    if (g == NULL) {
        printf("Graph doesn't exist!\n");
        return 1;
    }

    printf("Write the data in this order: sity1, sity2, lenWay\n");
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
        cleaning(g, NULL, 0, NULL);
        return 1;
    }

    State* states = malloc(k * sizeof(State));
    int* sitiesOwners = calloc(n + 1, sizeof(int));

    if (states == NULL || sitiesOwners == NULL) {
        printf("Allocate error!\n");
        cleaning(g, states, 0, sitiesOwners);
        return 1;
    }

    for (int i = 0; i < k; i++) {
        int capital;
        printf("Select the capitals: ");
        if (scanf("%d", &capital) != 1) {
            break;
        }

        states[i].id = i + 1;
        states[i].count = 0;
        states[i].sities = malloc(n * sizeof(int));

        if (states[i].sities == NULL) {
            cleaning(g, states, i, sitiesOwners);
            return 1;
        }

        states[i].sities[states[i].count++] = capital;
        sitiesOwners[capital] = i + 1;
    }

    annex(g, states, k, sitiesOwners);

    for (int i = 0; i < k; i++) {
        printf("State #%d: ", states[i].id);
        for (int j = 0; j < states[i].count; j++) {
            printf("%d ", states[i].sities[j]);
        }
        printf("\n");
    }

    cleaning(g, states, k, sitiesOwners);

    return 0;
}
