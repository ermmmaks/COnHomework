#include "states.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE* file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error at the opening file!\n");
        return 1;
    }

    int citiesCount;
    int roadsCount;
    if (fscanf(file, "%d %d", &citiesCount, &roadsCount) != 2) {
        fclose(file);
        return 0;
    }

    Graph* graph = graphCreate(citiesCount, roadsCount);
    if (graph == NULL) {
        printf("Graph doesn't exist!\n");
        return 1;
    }

    for (int i = 0; i < roadsCount; i++) {
        int city1;
        int city2;
        int len;
        if (fscanf(file, "%d %d %d", &city1, &city2, &len) != 3) {
            break;
        }
        graphAdd(graph, city1, city2, len, 2 * i);
        graphAdd(graph, city2, city1, len, 2 * i + 1);
    }

    int capitalsCount;
    if (fscanf(file, "%d", &capitalsCount) != 1) {
        freeAnnexTask(graph, NULL, 0, NULL);
        return 1;
    }

    State* states = statesCreate(capitalsCount);
    int* citiesOwners = calloc(citiesCount + 1, sizeof(int));

    if (states == NULL || citiesOwners == NULL) {
        printf("Allocate error!\n");
        freeAnnexTask(graph, states, 0, citiesOwners);
        return 1;
    }

    for (int i = 0; i < capitalsCount; i++) {
        int capital;
        if (fscanf(file, "%d", &capital) != 1) {
            break;
        }

        stateInit(states, i, i + 1, capital, citiesCount);
        citiesOwners[capital] = i + 1;
    }

    fclose(file);

    annex(graph, states, capitalsCount, citiesOwners);

    for (int i = 0; i < capitalsCount; i++) {
        printf("State #%d: ", stateGetId(states, i));
        for (int j = 0; j < stateGetCount(states, i); j++) {
            printf("%d ", stateGetCity(states, i, j));
        }
        printf("\n");
    }

    freeAnnexTask(graph, states, capitalsCount, citiesOwners);

    return 0;
}
