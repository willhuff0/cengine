//
// Created by Will on 7/7/24.
//

#include "input.h"

#include "common.h"
#include "engine.h"
#include <pthread.h>

typedef struct {
    float cursorX;
    float cursorY;

    int* keysDown;
    int* keysHeld;
    int* keysUp;
} InputState;

static pthread_mutex_t mutex;

static InputState* current;
static InputState* next;

static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
    next->cursorX = xPos;
    next->cursorY = yPos;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (action) {
        case GLFW_PRESS:
            arrput(next->keysDown, key);
            arrput(next->keysHeld, key);
            break;
        case GLFW_RELEASE:
            arrput(next->keysUp, key);
            break;
        case GLFW_REPEAT:
            break;
        default: break; // Unreachable
    }
}

static void initInputState(InputState* state) {
    state->cursorX = 0.0f;
    state->cursorY = 0.0f;

    state->keysDown = NULL;
    state->keysHeld = NULL;
    state->keysUp = NULL;
}

static void freeInputState(InputState* state) {
    if (state->keysDown != NULL) arrfree(state->keysDown);
    if (state->keysHeld != NULL) arrfree(state->keysHeld);
    if (state->keysUp != NULL) arrfree(state->keysUp);
}

void initInput() {
    pthread_mutex_init(&mutex, NULL);

    current = malloc(sizeof(InputState));
    next = malloc(sizeof(InputState));
    initInputState(current);
    initInputState(next);

    glfwSetInputMode(engine.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) glfwSetInputMode(engine.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetCursorPosCallback(engine.window, cursorPosCallback);
    glfwSetKeyCallback(engine.window, keyCallback);
}
void freeInput() {
    pthread_mutex_lock(&mutex);

    freeInputState(current);
    freeInputState(next);
    free(current);
    free(next);

    pthread_mutex_destroy(&mutex);
}


void inputSwapStates() {
    pthread_mutex_lock(&mutex);

    glfwSetCursorPos(engine.window, 0.0, 0.0);

    InputState* tempCurrent = current;
    current = next;
    next = tempCurrent;

    freeInputState(next);
    initInputState(next);

    for (int i = arrlen(current->keysHeld) - 1; i >= 0; --i) {
        int key = current->keysHeld[i];
        if (glfwGetKey(engine.window, key) == GLFW_PRESS) arrput(next->keysHeld, key);
    }

    pthread_mutex_unlock(&mutex);
}

float inputGetMouseDeltaX() { return current->cursorX; }
float inputGetMouseDeltaY() { return current->cursorY; }
void inputGetMouseDelta(vec2 dest) {
    dest[0] = current->cursorX;
    dest[1] = current->cursorY;
}

static bool arrContains(int* arr, int key) {
    for (int i = 0; i < arrlen(arr); ++i) {
        if (arr[i] == key) return true;
    }
    return false;
}

bool inputGetKeyDown(int key) { return arrContains(current->keysDown, key); }
bool inputGetKeyHeld(int key) { return arrContains(current->keysHeld, key); }
bool inputGetKeyUp(int key) { return arrContains(current->keysUp, key); }