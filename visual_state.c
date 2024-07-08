//
// Created by Will on 7/7/24.
//

#include "visual_state.h"

#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t mutex;

static VisualState* previous;
static VisualState* current;
static VisualState* next;

static void initVisualState(VisualState* state) { }

void initVisualStates() {
    pthread_mutex_init(&mutex, NULL);

    previous = malloc(sizeof(VisualState));
    current = malloc(sizeof(VisualState));
    next = malloc(sizeof(VisualState));
    initVisualState(previous);
    initVisualState(current);
    initVisualState(next);
}

void freeVisualStates() {
    pthread_mutex_lock(&mutex);

    free(previous);
    free(current);
    free(next);

    pthread_mutex_destroy(&mutex);
}

void swapVisualStates() {
    pthread_mutex_lock(&mutex);

    VisualState* tempPreviousState = previous;
    previous = current;
    current = next;
    next = tempPreviousState;

    pthread_mutex_unlock(&mutex);
}

const VisualState* getPreviousVisualState() { return previous; }
const VisualState* getCurrentVisualState() { return current; }
VisualState* getNextVisualState() { return next; }