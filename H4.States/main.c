#include "states.h"
#include <stdio.h>
#include <stdlib.h>

struct State {
    int id;
    int* cities;
    int count;
};

int main(void)
{
    int citiesCount;
    int roadsCount;
    printf("Write the count of cities and roads: ");
    if (scanf("%d %d", &citiesCount, &roadsCount) != 2) {
        return 0;
    }

    Graph* g = graphCreate(citiesCount, roadsCount);
    if (g == NULL) {
        printf("Graph doesn't exist!\n");
        return 1;
    }

    printf("Write the data in this order: city1, city2, lenRoad\n");
    for (int i = 0; i < roadsCount; i++) {
        int city1;
        int city2;
        int len;
        if (scanf("%d %d %d",  city1, &city2, &len) != 3) {
            break;
        }
        graphAdd(g, city1, city2, len, 2 * i);
        graphAdd(g, city2, city1, len, 2 * i + 1);
    }

    int k;
    if (scanf("%d", &k) != 1) {
        freeAnnexTask(g, NULL, 0, NULL);
        return 1;
    }

    State* states = malloc(k * sizeof(State));
    int* citiesOwners = calloc(citiesCount + 1, sizeof(int));

    if (states == NULL || citiesOwners == NULL) {
        printf("Allocate error!\n");
        freeAnnexTask(g, states, 0, citiesOwners);
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
        states[i].cities = malloc(citiesCount * sizeof(int));

        if (states[i].cities == NULL) {
            freeAnnexTask(g, states, i, citiesOwners);
            return 1;
        }

        states[i].cities[states[i].count++] = capital;
        citiesOwners[capital] = i + 1;
    }

    annex(g, states, k, citiesOwners);

    for (int i = 0; i < k; i++) {
        printf("State #%d: ", states[i].id);
        for (int j = 0; j < states[i].count; j++) {
            printf("%d ", states[i].cities[j]);
        }
        printf("\n");
    }

    freeAnnexTask(g, states, k, citiesOwners);

    return 0;
}
