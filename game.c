//
// Created by Will on 7/7/24.
//

#include "game.h"

#include "common.h"
#include "game_state.h"
#include "visual_state.h"
#include "input.h"

static bool shutdown;

GameTickArgs tickArgs;

static void gameInit() {
    initInput();
    initGameState();
}

static float lastTime = 0.0f;
static void setTickArgs() {
    // float currentTime = glfwGetTime();
    // if (lastTime < 0.001f) {
    //     tickArgs.deltaTime = GAME_TIMESTEP;
    // } else {
    //     tickArgs.deltaTime = currentTime - lastTime;
    // }
    // lastTime = currentTime;
    tickArgs.deltaTime = GAME_TIMESTEP;
}

static void gameTick() {
    setTickArgs();

    // network tick, sync
    // physics tick

    inputSwapStates();
    tickGameState();

    visualizeGameState(getNextVisualState());
    swapVisualStates();
}

static void gameFree() {
    freeGameState();
    freeInput();
}

static void sleepForSeconds(double seconds) {
    struct timespec req;
    req.tv_sec = (time_t)seconds;
    req.tv_nsec = (long)((seconds - req.tv_sec) * 1e9);

    nanosleep(&req, NULL);
}

static double startTime;
static double getTime() {
    return glfwGetTime() - startTime;
}

void* gameThreadEntry(void* _) {
    gameInit();

    startTime = glfwGetTime();
    unsigned int tickIndex = 0;
    for(;;) {
        int i = 0;
        while (GAME_TIMESTEP * tickIndex < getTime()) {
            gameTick();

            tickIndex++;
            i++;
            if (i >= 4) {
                int skip = ceil((getTime() - GAME_TIMESTEP * tickIndex) / GAME_TIMESTEP);
                tickIndex += skip;
                printf("[GAME] Running behind!, skipping %d tick(s).\n", skip);
                break;
            }

            if (shutdown) {
                gameFree();
                return NULL;
            }
        }

        double wait = GAME_TIMESTEP * tickIndex - getTime();
#ifdef DEBUG_TIMESTEP
        printf("TICK %d: tt=%f, ct=%f, executed %d time(s), waiting %f seconds\n", tickIndex, GAME_TIMESTEP * tickIndex, getTime(), i, wait);
#endif
        sleepForSeconds(wait);

        if (shutdown) {
            gameFree();
            return NULL;
        }
    }
}

void gameThreadActivateShutdownFlag() {
    shutdown = true;
}