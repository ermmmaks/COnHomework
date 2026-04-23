#include "states.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct State {
    int id;
    int* cities;
    int count;
};

void simpleTest(void)
{
    printf("First test is running\n");
    int citiesCount = 3;
    int roadsCount = 3;
    int capitalsCount = 2;

    Graph* g = graphCreate(citiesCount, roadsCount);
    graphAdd(g, 1, 2, 10, 0);
    graphAdd(g, 2, 1, 10, 1);
    graphAdd(g, 2, 3, 5, 2);
    graphAdd(g, 3, 2, 5, 3);
    graphAdd(g, 3, 2, 5, 4);
    graphAdd(g, 2, 3, 5, 5);

    State* states = malloc(capitalsCount * sizeof(State));
    int* citiesOwners = calloc(citiesCount + 1, sizeof(int));
    if (states == NULL || citiesOwners == NULL) {
        printf("Allocation error!\n");
        freeAnnexTask(g, states, capitalsCount, citiesOwners);
        return;
    }

    int capitals[] = { 1, 3 };
    for (int i = 0; i < capitalsCount; i++) {
        states[i].id = i + 1;
        states[i].count = 0;
        states[i].cities = malloc(citiesCount * sizeof(int));
        if (states[i].cities == NULL) {
            printf("Allocation error!\n");
            freeAnnexTask(g, states, capitalsCount, citiesOwners);
            return;
        }

        states[i].cities[states[i].count++] = capitals[i];
        citiesOwners[capitals[i]] = i + 1;
    }

    annex(g, states, capitalsCount, citiesOwners);

    assert(citiesOwners[1] == 1);
    assert(citiesOwners[3] == 2);
    assert(citiesOwners[2] == 1);

    printf("Simple test passed!\n");

    freeAnnexTask(g, states, capitalsCount, citiesOwners);
}

void isolatedTest(void)
{
    printf("Second test is running\n");

    int citiesCount = 3;
    int roadsCount = 1;
    int capitalsCount = 1;

    Graph* g = graphCreate(citiesCount, roadsCount);
    graphAdd(g, 1, 2, 10, 0);
    graphAdd(g, 2, 1, 10, 1);

    State* states = malloc(capitalsCount * sizeof(State));
    int* citiesOwners = calloc(citiesCount + 1, sizeof(int));
    if (states == NULL || citiesOwners == NULL) {
        printf("Allocation error!\n");
        freeAnnexTask(g, states, capitalsCount, citiesOwners);
        return;
    }

    states[0].id = 1;
    states[0].count = 0;
    states[0].cities = malloc(citiesCount * sizeof(int));
    states[0].cities[states[0].count++] = 1;
    citiesOwners[1] = 1;

    annex(g, states, capitalsCount, citiesOwners);

    assert(states[0].count == 2);
    assert(citiesOwners[3] == 0);

    printf("Test with isolated sity passed!\n");

    freeAnnexTask(g, states, capitalsCount, citiesOwners);
}

void zeroTest(void)
{
    printf("Third test is running\n");

    int citiesCount = 3;
    int roadsCount = 2;
    int capitalsCount = 1;

    Graph* g = graphCreate(citiesCount, roadsCount);
    graphAdd(g, 1, 2, 0, 0);
    graphAdd(g, 2, 1, 0, 1);
    graphAdd(g, 1, 3, 0, 2);
    graphAdd(g, 3, 1, 0, 3);

    State* states = malloc(capitalsCount * sizeof(State));
    int* citiesOwners = calloc(citiesCount + 1, sizeof(int));
    if (states == NULL || citiesOwners == NULL) {
        printf("Allocation error!\n");
        freeAnnexTask(g, states, capitalsCount, citiesOwners);
        return;
    }

    states[0].id = 1;
    states[0].count = 0;
    states[0].cities = malloc(citiesCount * sizeof(int));
    states[0].cities[states[0].count++] = 1;
    citiesOwners[1] = 1;

    annex(g, states, capitalsCount, citiesOwners);

    assert(citiesOwners[2] == 1);
    assert(citiesOwners[3] == 1);
    assert(states[0].count == 3);

    printf("Test with zero ways passed!\n");

    freeAnnexTask(g, states, capitalsCount, citiesOwners);
}

int main(void)
{
    simpleTest();
    isolatedTest();
    zeroTest();
    printf("___ All tests passed!!! ___\n");

    return 0;
}
