#include "states.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct State {
    int id;
    int* sities;
    int count;
};

void simpleTest(void)
{
    printf("First test is running\n");
    int n = 3;
    int m = 3;
    int k = 2;

    Graph* g = graphCreate(n, m);
    graphAdd(g, 1, 2, 10, 0);
    graphAdd(g, 2, 1, 10, 1);
    graphAdd(g, 2, 3, 5, 2);
    graphAdd(g, 3, 2, 5, 3);
    graphAdd(g, 3, 2, 5, 4);
    graphAdd(g, 2, 3, 5, 5);

    State* states = malloc(k * sizeof(State));
    int* sitiesOwners = calloc(n + 1, sizeof(int));
    if (states == NULL || sitiesOwners == NULL) {
        printf("Allocation error!\n");
        cleaning(g, states, k, sitiesOwners);
        return;
    }

    int capitals[] = { 1, 3 };
    for (int i = 0; i < k; i++) {
        states[i].id = i + 1;
        states[i].count = 0;
        states[i].sities = malloc(n * sizeof(int));
        if (states[i].sities == NULL) {
            printf("Allocation error!\n");
            cleaning(g, states, k, sitiesOwners);
            return;
        }

        states[i].sities[states[i].count++] = capitals[i];
        sitiesOwners[capitals[i]] = i + 1;
    }

    annex(g, states, k, sitiesOwners);

    assert(sitiesOwners[1] == 1);
    assert(sitiesOwners[3] == 2);
    assert(sitiesOwners[3] == 2);

    printf("Simple test passed!\n");

    cleaning(g, states, k, sitiesOwners);
}

void isolatedTest(void)
{
    printf("Second test is running\n");

    int n = 3;
    int m = 1;
    int k = 1;

    Graph* g = graphCreate(n, m);
    graphAdd(g, 1, 2, 10, 0);
    graphAdd(g, 2, 1, 10, 1);

    State* states = malloc(k * sizeof(State));
    int* sitiesOwners = calloc(n + 1, sizeof(int));
    if (states == NULL || sitiesOwners == NULL) {
        printf("Allocation error!\n");
        cleaning(g, states, k, sitiesOwners);
        return;
    }

    states[0].id = 1;
    states[0].count = 0;
    states[0].sities = malloc(n * sizeof(int));
    states[0].sities[states[0].count++] = 1;
    sitiesOwners[1] = 1;

    annex(g, states, k, sitiesOwners);

    assert(states[0].count == 2);
    assert(sitiesOwners[3] == 0);

    printf("Test with isolated sity passed!\n");

    cleaning(g, states, k, sitiesOwners);
}

void zeroTest(void)
{
    printf("Third test is running\n");

    int n = 3;
    int m = 2;
    int k = 1;

    Graph* g = graphCreate(n, m);
    graphAdd(g, 1, 2, 0, 0);
    graphAdd(g, 2, 1, 0, 1);
    graphAdd(g, 1, 3, 0, 2);
    graphAdd(g, 3, 1, 0, 3);

    State* states = malloc(k * sizeof(State));
    int* sitiesOwners = calloc(n + 1, sizeof(int));
    if (states == NULL || sitiesOwners == NULL) {
        printf("Allocation error!\n");
        cleaning(g, states, k, sitiesOwners);
        return;
    }

    states[0].id = 1;
    states[0].count = 0;
    states[0].sities = malloc(n * sizeof(int));
    states[0].sities[states[0].count++] = 1;
    sitiesOwners[1] = 1;

    annex(g, states, k, sitiesOwners);

    assert(sitiesOwners[2] == 1);
    assert(sitiesOwners[3] == 1);
    assert(states[0].count == 3);

    printf("Test with zero ways passed!\n");

    cleaning(g, states, k, sitiesOwners);
}

int main(void)
{
    simpleTest();
    isolatedTest();
    zeroTest();
    printf("___ All tests passed!!! ___\n");

    return 0;
}
