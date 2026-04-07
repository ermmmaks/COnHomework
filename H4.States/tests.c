#include "states.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct State {
    int id;
    int* sityes;
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
    int* sityesOwners = calloc(n + 1, sizeof(int));
    if (states == NULL || sityesOwners == NULL) {
        printf("Allocation error!\n");
        return 1;
    }

    int capitals[] = { 1, 3 };
    for (int i = 0; i < k; i++) {
        states[i].id = i + 1;
        states[i].count = 0;
        states[i].sityes = malloc(n * sizeof(int));
        if (states[i].sityes == NULL) {
            printf("Allocation error!\n");
            return 1;
        }

        states[i].sityes[states[i].count++] = capitals[i];
        sityesOwners[capitals[i]] = i + 1;
    }

    annexation(g, states, k, sityesOwners);

    assert(sityesOwners[1] == 1);
    assert(sityesOwners[3] == 2);
    assert(sityesOwners[3] == 2);

    printf("Simple test passed!\n");

    globalFree(g, states, k, sityesOwners);
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
    int* sityesOwners = calloc(n + 1, sizeof(int));
    if (states == NULL || sityesOwners == NULL) {
        printf("Allocation error!\n");
        return 1;
    }

    states[0].id = 1;
    states[0].count = 0;
    states[0].sityes = malloc(n * sizeof(int));
    states[0].sityes[states[0].count++] = 1;
    sityesOwners[1] = 1;

    annexation(g, states, k, sityesOwners);

    assert(states[0].count == 2);
    assert(sityesOwners[3] == 0);

    printf("Test with isolated sity passed!\n");

    globalFree(g, states, k, sityesOwners);
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
    int* sityesOwners = calloc(n + 1, sizeof(int));
    if (states == NULL || sityesOwners == NULL) {
        printf("Allocation error!\n");
        return 1;
    }

    states[0].id = 1;
    states[0].count = 0;
    states[0].sityes = malloc(n * sizeof(int));
    states[0].sityes[states[0].count++] = 1;
    sityesOwners[1] = 1;

    annexation(g, states, k, sityesOwners);

    assert(sityesOwners[2] == 1);
    assert(sityesOwners[3] == 1);
    assert(states[0].count == 3);

    printf("Test with zero ways passed!\n");

    globalFree(g, states, k, sityesOwners);
}

int main(void)
{
    simpleTest();
    isolatedTest();
    zeroTest();
    printf("___ All tests passed!!! ___\n");

    return 0;
}
